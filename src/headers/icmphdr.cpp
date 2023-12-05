#include "icmphdr.h"

uint16_t IcmpHdr::calcIcmpChecksum(IpHdr *iphdr, IcmpHdr *icmphdr) {
    uint32_t ret = 0;
    icmphdr->checksum_ = 0;

    uint16_t len = iphdr->len() - iphdr->ipHdrSize();
    uint16_t *pword = reinterpret_cast<uint16_t *>(icmphdr);
    for (int i = 0; i < len / 2; i++) {
        ret += htons(*pword);
        pword++;
    }
    if (len & 1)
        ret += uint32_t(*(reinterpret_cast<uint8_t *>(pword)) << 8);

    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}