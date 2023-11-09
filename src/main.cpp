#include "pch.h"

#include "packet.h"
#include "utility.h"
#include "gtrace.h"
#include "rawsock.h"

#include "filters/filter.h"
#include "filters/tcpackfilter.h"

void usage() {
	printf("usage: sudo ./block-packet <mirror interface> <send interface> [sni list]\n");
	printf("example: sudo ./block-packet enp0s3 eth0\n");
	printf("example: sudo ./block-packet enp0s3 eth0 sni.txt\n");
}

int main(int argc, char* argv[]) {
	gtrace_default("127.0.0.1", 8908, 1, "block-packet.log");
	GTRACE("Logging level : Message");

	if(argc < 3) {
		usage();
		return -1;
	}
	char *mirrorInterface = argv[1];
	char *sendInterface = argv[2];
	std::unordered_set<std::string> sniList;
	if(argc == 4) {
		bool success = loadSni(argv[3], sniList);
		if(!success) {
			return -1;
		}
	}

	pcap_t *mirrorPcap = openPcap(mirrorInterface);
	if(mirrorPcap == NULL) return -1;

	RawSock sendSocket;
	if(!sendSocket.open(sendInterface)) return -1;
	
	int pktCnt = 0;
	RxPacket *rxPacket = new RxPacket;
	TxPacket *fwd = new TxPacket;
	TxPacket *bwd = new TxPacket;
	struct pcap_pkthdr *header;
	const uint8_t *packet;
	TcpAckFilter tcpackfilter;
	while(true) {
		int res = pcap_next_ex(mirrorPcap, &header, &packet);
		if(res == 0) {
			usleep(1);
			continue;
		}

		pktCnt++;

		// initialize
		/////////////    rxPacket->parse(packet);

		// filter
		// bool isTcpAck(RxPacket *pkt) {
		// 	if(pkt->ethhdr->type() != EthHdr::ipv4) return false;
		// 	if(pkt->iphdr->proto() != IpHdr::tcp) return false;
		// 	if(pkt->tcphdr->flags() != TcpHdr::flagsAck) return false;
			
		// 	return true;
		// }
		
		// if(!isTcpAck(rxPacket)) continue;
		if(tcpackfilter.parseAndFilter(packet)) {
			tcpackfilter.blocker(sendSocket);
		}
		else {
			continue;
		}

		// print counter
		GTRACE("========%d========", pktCnt);
	}

	delete rxPacket;
	delete fwd;
	delete bwd;
	pcap_close(mirrorPcap);
	sendSocket.close();
	
	return 0;
}
