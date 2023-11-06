#include "tcphdr.h"

uint16_t TcpHdr::payloadLen(IpHdr *iphdr, TcpHdr *tcphdr) {
    return iphdr->len() - iphdr->ip_size() - tcphdr->tcpHdrSize();
}

/**
 * Q. Why didn't you add payload? To calculate checksum, you should add not only header but also payload.
 * A. Because this function is only used in making TxPacket that has no payload.
 *    Same reason, padding is also ignored.
*/
uint16_t TcpHdr::calcTcpChecksum(IpHdr *iphdr, TcpHdr *tcphdr) {
    uint32_t ret = 0;
    uint16_t *pword = reinterpret_cast<uint16_t*>(tcphdr);
    tcphdr->_checksum = 0;
    
    ret += (ntohs(iphdr->src() >> 16)) + (ntohs(iphdr->src() & 0xFFFF));
    ret += (ntohs(iphdr->dst() >> 16)) + (ntohs(iphdr->dst() & 0xFFFF));
    ret += iphdr->proto();
    ret += tcphdr->tcp_size();
    for(int i=0; i<tcphdr->tcpHdrSize(); i+=2) {
        ret += ntohs(*(pword + i));
    }
    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}