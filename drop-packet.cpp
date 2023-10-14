#define DEBUG

#include "headers.h"
#include "log.h"
#include "gtrace.h"

// C++
#include <fstream>
#include <vector>
#include <string>
#include <utility>

// C
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

#pragma pack(push, 1)
struct _tcpPacket final {
	struct _eth eth;
	struct _ip  ip;
	struct _tcp tcp;
};
#pragma pack(pop)

void usage() {
	printf("usage: sudo ./drop-packet <interface>\n");
	printf("example: sudo ./drop-packet enp0s3\n");
}

uint8_t hex2int(char n) {
	if('0' <= n && n <= '9') return n - '0';
	else return n - 'a' + 10;
}

uint8_t* str2arrayMac(std::string mac_str) {
	uint8_t *ret = (uint8_t*)malloc(sizeof(uint8_t) * 6);
	for(int i=0; i<6; i++) {
		ret[i] = hex2int(mac_str[i*3])*16 + hex2int(mac_str[i*3+1]);
	}
	return ret;
}

int main(int argc, char* argv[]) {
	GTRACE("Debug mode is enabled");
	if(argc != 2) {
		usage();
		return -1;
	}
	char* handle = argv[1];

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(handle, BUFSIZ, 1, 1, errbuf);
	if(pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", handle, errbuf);
		return -1;
	}

	std::ifstream iface("/sys/class/net/" + std::string(handle) + "/address");
	std::string mac_temp((std::istreambuf_iterator<char>(iface)), std::istreambuf_iterator<char>());
	std::string mac_str(mac_temp.begin(), mac_temp.end()-1);
#ifdef DEBUG
	printf("%s\n", mac_str.c_str());
#endif
	uint8_t *my_mac = str2arrayMac(mac_str);

	int cnt = 0;
	_tcpPacket *tcpPacket = new _tcpPacket;
	_tcpPacket *fwd = new _tcpPacket;
	_tcpPacket *bwd = new _tcpPacket;
	while(++cnt) {
		struct pcap_pkthdr* header;
		const uint8_t* packet;
		int res = pcap_next_ex(pcap, &header, &packet);
		if(res == 0) continue;

		tcpPacket->eth = *((struct _eth*)(packet));
		tcpPacket->ip  = *((struct _ip* )(packet + ETH_SIZE));
		tcpPacket->tcp = *((struct _tcp*)(packet + ETH_SIZE + tcpPacket->ip.ip_size()));

		if(ntohs(tcpPacket->eth.type) != tcpPacket->eth.IPv4) continue;
		if(tcpPacket->ip.proto != IP_PROTO_TCP) continue;
		if(tcpPacket->tcp.flags2 != TCP_FLAGS_ACK) continue;

		fwd->eth = bwd->eth = tcpPacket->eth;
		fwd->ip  = bwd->ip  = tcpPacket->ip;
		fwd->tcp = bwd->tcp = tcpPacket->tcp;


		//PRINT_TCP(fwd);
		//PRINT_TCP(bwd);

		for(int i=0; i<6; i++)
			fwd->eth.src[i] = bwd->eth.src[i] = my_mac[i];
		for(int i=0; i<6; i++)
			bwd->eth.dst[i] = tcpPacket->eth.src[i];

		uint16_t ip_size = tcpPacket->ip.hdr_len * 4;
		uint16_t tcp_size = tcpPacket->tcp.hdr_len * 4;
		fwd->ip.len = bwd->ip.len = ntohs(ip_size + tcp_size);
		std::swap(bwd->ip.src, bwd->ip.dst);
		bwd->ip.ttl = 128;

		std::swap(bwd->tcp.srcport, bwd->tcp.dstport);
		uint16_t tcp_data_size = ntohs(tcpPacket->ip.len) - ip_size - tcp_size;
		fwd->tcp.seq_raw = ntohl(tcpPacket->tcp.seq_raw + tcp_data_size);
		bwd->tcp.seq_raw = tcpPacket->tcp.seq_raw;
		fwd->tcp.flags2 = TCP_FLAGS_RSTACK;
		bwd->tcp.flags2 = TCP_FLAGS_RSTACK;


		fwd->ip.checksum = _ip::calcIpChecksum(&(fwd->ip));
		bwd->ip.checksum = _ip::calcIpChecksum(&(bwd->ip));

		fwd->tcp.checksum = _tcp::calcTcpChecksum(&(fwd->ip), &(fwd->tcp));
		bwd->tcp.checksum = _tcp::calcTcpChecksum(&(bwd->ip), &(bwd->tcp));

		res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(fwd), sizeof(_tcpPacket));
		GTRACE("send forward packet");
		if(res != 0) {
			fprintf(stderr, "pcap_sendpacket(%s) of fwd return null - %s\n", handle, errbuf);
		}
		res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(bwd), sizeof(_tcpPacket));
		GTRACE("send backward packet");
		if(res != 0) {
			fprintf(stderr, "pcap_sendpacket(%s) of bwd return null - %s\n", handle, errbuf);
		}

		printf("========%d========\n", cnt);
	}

	delete tcpPacket;
	pcap_close(pcap);
	
	return 0;
}
