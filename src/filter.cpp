#include "filter.h"

bool not_want_filter(RxOpenVpnTcpPacket *pkt) {
	if(pkt->eth->type() != EthHdr::ipv4) return true;
	if(pkt->ip->proto() != IpHdr::tcp) return true;
	
	// vs Proton VPN with Open VPN (TCP)
	if(pkt->tcp->payload_len(pkt->ip, pkt->tcp) != pkt->openvpntcp->plen() + 2) return true;
	if(pkt->openvpntcp->type() != 0x48) return true;
        
        return false;
}
