#include "filter.h"

bool custom_filter(RxOpenVpnTcpPacket *pkt) {
	printf("eth type = %d\n", pkt->eth->type());
	if(pkt->eth->type() != EthHdr::ipv4) return true;
	printf("ip proto = %d\n", pkt->ip->proto());
	if(pkt->ip->proto() != IpHdr::tcp) return true;
	
	printf("ip header size = %d\n", pkt->ip->ip_size());
	// vs Proton VPN with Open VPN (TCP)
	if(pkt->tcp->payload_len(pkt->ip, pkt->tcp) != pkt->openvpntcp->plen() + 2) return true;
	if(pkt->openvpntcp->type() != 0x48) return true;
        
        return false;
}
