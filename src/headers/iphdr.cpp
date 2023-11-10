#include "iphdr.h"

uint16_t IpHdr::calcIpChecksum(IpHdr *iphdr) {
    uint32_t ret = 0;
    uint16_t *pword = reinterpret_cast<uint16_t*>(iphdr);
    iphdr->checksum_ = 0;
    
    for(int i=0; i<iphdr->ipHdrSize(); i+=2) {
        ret += ntohs(*(pword + i));
    }
    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}