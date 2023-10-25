#include "tcp.h"

uint16_t TcpHdr::len(IpHdr *ip, TcpHdr *tcp) {
    return ip->len() - ip->ip_size() - tcp->tcp_size();
}

uint16_t TcpHdr::calcTcpChecksum(IpHdr *ip, TcpHdr *tcp) {
    uint32_t ret = 0;
    tcp->_checksum = 0;
    
    ret += (ntohs(ip->src() >> 16)) + (ntohs(ip->src() & 0xFFFF));
    ret += (ntohs(ip->dst() >> 16)) + (ntohs(ip->dst() & 0xFFFF));
    ret += ip->proto();
    ret += tcp->tcp_size();
    for(int i=0; i<tcp->tcp_size(); i+=2) {
        ret += (*((uint8_t*)tcp + i) << 8) + (*((uint8_t*)tcp + i + 1));
    }
    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}