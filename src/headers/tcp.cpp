#include "tcp.h"

uint16_t TcpHdr::payload_len(IpHdr *ip, TcpHdr *tcp) {
    return ip->len() - ip->ip_size() - tcp->tcp_size();
}

/**
 * Q. Why didn't you add payload? To calculate checksum, you should add not only header but also payload.
 * A. Because this function is only used in making TxPacket that has no payload.
 *    Same reason, padding is also ignored.
*/
uint16_t TcpHdr::calcTcpChecksum(IpHdr *ip, TcpHdr *tcp) {
    uint32_t ret = 0;
    uint16_t *pword = reinterpret_cast<uint16_t*>(tcp);
    tcp->_checksum = 0;
    
    ret += (ntohs(ip->src() >> 16)) + (ntohs(ip->src() & 0xFFFF));
    ret += (ntohs(ip->dst() >> 16)) + (ntohs(ip->dst() & 0xFFFF));
    ret += ip->proto();
    ret += tcp->tcp_size();
    for(int i=0; i<tcp->tcp_size(); i+=2) {
        ret += ntohs(*(pword + i));
    }
    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}