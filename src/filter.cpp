#include "filter.h"

bool isTcpAck(RxPacket *pkt) {
	if(pkt->ethhdr->type() != EthHdr::ipv4) return false;
	if(pkt->iphdr->proto() != IpHdr::tcp) return false;
	if(pkt->tcphdr->flags() != TcpHdr::flagsAck) return false;
	
	return true;
}

bool isTcpAck(RxOpenVpnTcpPacket *pkt) {
	if(pkt->ethhdr->type() != EthHdr::ipv4) return false;
	if(pkt->iphdr->proto() != IpHdr::tcp) return false;
	if(pkt->tcphdr->flags() != TcpHdr::flagsAck) return false;
	
	return true;
}

bool isOpenVpnTcp(RxOpenVpnTcpPacket *pkt) {
	if(!isTcpAck(pkt)) return false;
	if(pkt->tcphdr->payloadLen(pkt->iphdr, pkt->tcphdr) != pkt->openvpntcphdr->plen() + 2) return false;
	if(pkt->openvpntcphdr->type() != 0x48) return false;
    return true;
}
