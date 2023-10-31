#include "pch.h"

#include "packet.h"
#include "utility.h"
#include "gtrace.h"
#include "filter.h"
#include "rawsock.h"

void usage() {
	printf("usage: sudo ./block-packet <mirror interface> <send interface>\n");
	printf("example: sudo ./block-packet enp0s3 eth0\n");
}

int main(int argc, char* argv[]) {
	GTRACE("Logging level : Message");

	if(argc != 3) {
		usage();
		return -1;
	}
	char* mirror_interface = argv[1];
	char* send_interface = argv[2];

	pcap_t* mirror_pcap = open_pcap(mirror_interface);
	if(mirror_pcap == NULL) return -1;

	RawSock send_socket;
	if(!send_socket.open(send_interface)) return -1;
	
	int pkt_cnt = 0, res;
	RxOpenVpnTcpPacket *rxPacket = new RxOpenVpnTcpPacket;
	TxPacket *fwd = new TxPacket;
	TxPacket *bwd = new TxPacket;
	struct pcap_pkthdr* header;
	const uint8_t* packet;
	while(true) {
		if(!pcap_next_ex(mirror_pcap, &header, &packet)) {
			usleep(300);
			continue;
		}
		pkt_cnt++;

		// initialize
		rxPacket->eth = (struct EthHdr*)(packet);
		rxPacket->ip  = (struct IpHdr* )(packet + ETH_SIZE);
		rxPacket->tcp = (struct TcpHdr*)(packet + ETH_SIZE + rxPacket->ip->ip_size());
		rxPacket->openvpntcp = (struct OpenVpnTcpHdr*)(packet + ETH_SIZE + rxPacket->ip->ip_size() + rxPacket->tcp->tcp_size());

		// you can modify custom_filter() function
		// it must return true, when a packet is recieved what you don't need
		if(custom_filter(rxPacket)) continue;

		// copy packet
		fwd->ip  = bwd->ip  = *(rxPacket->ip);
		fwd->tcp = bwd->tcp = *(rxPacket->tcp);
		fwd->tcp._hdr_len = 5;

		// modify mac address
		// deleted bacause raw socket doesn't need mac

		// modify ip header
		fwd->ip._len = bwd->ip._len = ntohs(rxPacket->ip->ip_size() + rxPacket->tcp->tcp_size());
		std::swap(bwd->ip._src, bwd->ip._dst);
		bwd->ip._ttl = 128;

		// modify tcp header
		std::swap(bwd->tcp._srcport, bwd->tcp._dstport);
		fwd->tcp._seq_raw = ntohl(rxPacket->tcp->seq_raw() + TcpHdr::payload_len(rxPacket->ip, rxPacket->tcp));
		bwd->tcp._seq_raw = rxPacket->tcp->_seq_raw;
		fwd->tcp._flags = bwd->tcp._flags = TcpHdr::flags_rst | TcpHdr::flags_ack;

		// calculate ip and tcp checksum
		fwd->ip._checksum = IpHdr::calcIpChecksum(&(fwd->ip));
		bwd->ip._checksum = IpHdr::calcIpChecksum(&(bwd->ip));

		fwd->tcp._checksum = TcpHdr::calcTcpChecksum(&(fwd->ip), &(fwd->tcp));
		bwd->tcp._checksum = TcpHdr::calcTcpChecksum(&(bwd->ip), &(bwd->tcp));
		
		// send packet
		send_socket.sendto(fwd, VAR_NAME(fwd));
		send_socket.sendto(bwd, VAR_NAME(bwd));

		// print counter
		GTRACE("========%d========", pkt_cnt);
	}

	delete rxPacket;
	delete fwd;
	delete bwd;
	pcap_close(mirror_pcap);
	send_socket.close();
	
	return 0;
}
