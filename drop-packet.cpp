#define DEBUG

#include "headers.h"
#include "log.h"

// C++
#include <fstream>
#include <vector>
#include <string>
#include <utility>

// C
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

struct _tcpPacket final {
	struct _eth eth;
	struct _ip  ip;
	struct _tcp tcp;
};

void usage() {
	printf("usage: sudo ./drop-packet <interface>\n");
	printf("example: sudo ./drop-packet enp0s3\n");
}

uint16_t ntohs(uint16_t n) {
	return (n>>8) | (n<<8);
}

uint32_t ntohl(uint32_t n) {
	return (n<<24) | (n<<8 & 0xFF0000) | (n>>8 & 0xFF00) | (n>>24 & 0xFF);
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
	LOG("Debug mode is enabled");
	if(argc != 2) {
		usage();
		return -1;
	}
	char* handle = argv[1];

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(handle, BUFSIZ, 1, 1000, errbuf);
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
		tcpPacket->tcp = *((struct _tcp*)(packet + ETH_SIZE + IP_SIZE));

		if(ntohs(tcpPacket->eth.type) != ETH_TYPE_IPV4) continue;
		if(tcpPacket->ip.proto != IP_PROTO_TCP) continue;
		if(tcpPacket->tcp.flags2 != TCP_FLAGS_ACK) continue;

		fwd->eth = bwd->eth = tcpPacket->eth;
		fwd->ip  = bwd->ip  = tcpPacket->ip;
		fwd->tcp = bwd->tcp = tcpPacket->tcp;

		for(int i=0; i<6; i++)
			fwd->eth.src[i] = my_mac[i];
		for(int i=0; i<6; i++)
			bwd->eth.dst[i] = bwd->eth.src[i];
		for(int i=0; i<6; i++)
			bwd->eth.src[i] = my_mac[i];

		fwd->ip.len = bwd->ip.len = IP_SIZE + fwd->tcp.hdr_len;
		std::swap(bwd->ip.src, bwd->ip.dst);

		PRINT_TCP(fwd);
		PRINT_TCP(bwd);
		printf("========%d========\n", cnt);
	}

	delete tcpPacket;
	
	return 0;
}
