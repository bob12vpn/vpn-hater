#include "utility.h"

pcap_t* open_pcap(char* interface) {
	char errbuf[PCAP_ERRBUF_SIZE];	
	pcap_t* pcap = pcap_open_live(interface, BUFSIZ, 1, -1, errbuf);
	if(pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", interface, errbuf);
		return NULL;
	}
	return pcap;
}

int open_raw_socket(char* interface) {
	int send_socket = ::socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
	if(send_socket == -1) {
		GTRACE("socket return -1");
		return -1;
	}

	int one = 1;
	int* pOne = &one;
	int res = ::setsockopt(send_socket, IPPROTO_IP, IP_HDRINCL, pOne, sizeof(one));
	if(res < 0) {
		GTRACE("setsockopt(IP_HDRINCL) Failed");
		return -1;
	}

	if(strlen(interface) != 0) {
		res = ::setsockopt(send_socket, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));
		if(res < 0) {
			GTRACE("setsockopt(SO_BINDTODEVICE) Failed");
			return -1;
		}
	}

	return send_socket;
}

void send_packet(int socket, struct sockaddr_in addr_in_, TxPacket *pkt) {
   	addr_in_.sin_addr.s_addr = pkt->ip.dst();
	// int res = 
	::sendto(socket, &(pkt->ip), pkt->ip.len(), 0, (struct sockaddr*)&addr_in_, sizeof(struct sockaddr_in));
}

uint8_t hex2int(char n) {
	return ('0' <= n && n <= '9') ? n - '0' : n - 'a' + 10;
}

bool custom_filter(RxOpenVpnTcpPacket *pkt) {
	if(pkt->eth->type() != EthHdr::ipv4) return true;
	if(pkt->ip->proto() != IpHdr::tcp) return true;
	if(pkt->tcp->flags() != TcpHdr::flags_ack) return true;
	
	// vs Proton VPN with Open VPN (TCP)
	//if(pkt->tcp->payload_len(pkt->ip, pkt->tcp) != pkt->openvpntcp->plen() + 2) return true;
	//if(pkt->openvpntcp->type() != 0x48) return true;
	
	return false;
}
