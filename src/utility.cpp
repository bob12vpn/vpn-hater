#include "utility.h"

pcap_t* open_pcap(char* interface) {
	pcap_t* pcap = pcap_open_live(interface, BUFSIZ, 1, -1, errbuf);
	if(pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", interface, errbuf);
		return NULL;
	}
	return pcap;
}

auto open_raw_socket(char* interface) {
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

	if(interface != "") {
		res = ::setsockopt(send_socket, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));
		if(res < 0) {
			GTRACE("setsockopt(SO_BINDTODEVICE) Failed");
			return -1;
		}
	}
}

void send_packet(int socket, struct sockaddr_in addr_in_, TxPacket *pkt) {
    addr_in_.sin_addr.s_addr = pkt->ip.dst();
    int res = ::sendto(socket, &(pkt->ip), pkt->ip.len(), 0, (struct sockaddr*)&addr_in_, sizeof(struct sockaddr_in));
}

uint8_t hex2int(char n) {
	return ('0' <= n && n <= '9') ? n - '0' : n - 'a' + 10;
}

uint8_t* resolve_mac(char* interface) {
	std::ifstream iface("/sys/class/net/" + std::string(interface) + "/address");
	std::string mac((std::istreambuf_iterator<char>(iface)), std::istreambuf_iterator<char>());
	mac.pop_back();
	uint8_t *ret = (uint8_t*)malloc(sizeof(uint8_t) * 8);
	for(int i=0; i<6; i++) {
		ret[i] = hex2int(mac[i*3])*16 + hex2int(mac[i*3+1]);
	}
	return ret;
}

bool custom_filter(RxOpenVpnTcpPacket *pkt) {
	if(pkt->eth.type() != EthHdr::ipv4) return true;
	if(pkt->ip.proto() != IpHdr::tcp) return true;
	// if(pkt->tcp.flags2() != TcpHdr::flags_psh | TcpHdr::flags_ack) return true;
	
	// vs Proton VPN with Open VPN (TCP)
	if(pkt->tcp.len(&(pkt->ip), &(pkt->tcp)) != pkt->openvpn.plen() + 2) return true;
	if(pkt->openvpn.type() != 0x48) return true;
	
	return false;
}