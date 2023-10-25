#include "ip.h"

uint16_t IpHdr::calcIpChecksum(IpHdr *ip) {
    uint32_t ret = 0;
    uint16_t *tmp = reinterpret_cast<uint16_t*>(ip);
    ip->_checksum = 0;
    for(int i=0; i<ip->hdr_len(); i+=2) {
        ret += ntohs(*(tmp + i));
    }
    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}