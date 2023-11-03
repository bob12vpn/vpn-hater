#include "filter.h"

bool isTcp(RxPacket *pkt) {
	if(pkt->eth->type() != EthHdr::ipv4) return false;
	if(pkt->ip->proto() != IpHdr::tcp) return false;
	
	return true;
}

bool isTcp(RxOpenVpnTcpPacket *pkt) {
	if(pkt->eth->type() != EthHdr::ipv4) return false;
	if(pkt->ip->proto() != IpHdr::tcp) return false;
	
	return true;
}

bool isOpenVpnTcp(RxOpenVpnTcpPacket *pkt) {
	if(!isTcp(pkt)) return false;
	if(pkt->tcp->payload_len(pkt->ip, pkt->tcp) != pkt->openvpntcp->plen() + 2) return false;
	if(pkt->openvpntcp->type() != 0x48) return false;
    return true;
}
