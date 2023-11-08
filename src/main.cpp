#include "pch.h"

#include "packet.h"
#include "utility.h"
#include "gtrace.h"
#include "filter.h"
#include "rawsock.h"

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
	RxOpenVpnTcpPacket *rxPacket = new RxOpenVpnTcpPacket;
	TxPacket *fwd = new TxPacket;
	TxPacket *bwd = new TxPacket;
	struct pcap_pkthdr *header;
	const uint8_t *packet;
	while(true) {
		int res = pcap_next_ex(mirrorPcap, &header, &packet);
		if(res == 0) {
			usleep(1);
			continue;
		}

		pktCnt++;

		// initialize
		rxPacket->parse(packet);

		// filter
		if(!isTcpAck(rxPacket)) continue;

		// copy packet
		fwd->iphdr  = bwd->iphdr  = *(rxPacket->iphdr);
		fwd->tcphdr = bwd->tcphdr = *(rxPacket->tcphdr);
		fwd->tcphdr.hdrLen_ = bwd->tcphdr.hdrLen_ = 5;

		// modify mac address
		// deleted bacause raw socket doesn't need mac

		// modify ip header
		fwd->iphdr.len_ = bwd->iphdr.len_ = ntohs(40);
		std::swap(bwd->iphdr.src_, bwd->iphdr.dst_);
		bwd->iphdr.ttl_ = 128;

		// modify tcp header
		std::swap(bwd->tcphdr.srcport_, bwd->tcphdr.dstport_);
		fwd->tcphdr.seqRaw_ = ntohl(rxPacket->tcphdr->seqRaw() + TcpHdr::payloadLen(rxPacket->iphdr, rxPacket->tcphdr));
		bwd->tcphdr.seqRaw_ = rxPacket->tcphdr->seqRaw_;
		fwd->tcphdr.flags_ = bwd->tcphdr.flags_ = TcpHdr::flagsRst | TcpHdr::flagsAck;

		// calculate ip and tcp checksum
		fwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(fwd->iphdr));
		bwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(bwd->iphdr));

		fwd->tcphdr.checksum_ = TcpHdr::calcTcpChecksum(&(fwd->iphdr), &(fwd->tcphdr));
		bwd->tcphdr.checksum_ = TcpHdr::calcTcpChecksum(&(bwd->iphdr), &(bwd->tcphdr));
		
		// send packet
		sendSocket.sendto(fwd);
		sendSocket.sendto(bwd);

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
