#include "iphdr.h"

uint16_t IpHdr::calcIpChecksum(IpHdr *iphdr) {
    uint32_t ret = 0;
    iphdr->checksum_ = 0;

    uint16_t len = iphdr->ipHdrSize();
    uint16_t *pword = reinterpret_cast<uint16_t *>(iphdr);
    for (int i = 0; i < len / 2; i++) {
        ret += ntohs(*pword);
        pword++;
    }

    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}