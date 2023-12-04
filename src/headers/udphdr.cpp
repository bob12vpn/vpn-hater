#include "udphdr.h"

uint16_t UdpHdr::calcUdpChecksum(IpHdr *iphdr, UdpHdr *udphdr) {
    uint32_t ret = 0;
    udphdr->checksum_ = 0;

    ret += (ntohs(iphdr->src() >> 16)) + (ntohs(iphdr->src() & 0xFFFF));
    ret += (ntohs(iphdr->dst() >> 16)) + (ntohs(iphdr->dst() & 0xFFFF));
    ret += iphdr->proto();
    ret += udphdr->length();

    uint16_t len = udphdr->length();
    uint16_t *pword = reinterpret_cast<uint16_t *>(udphdr);
    for (int i = 0; i < len / 2; i++) {
        ret += htons(*pword);
        pword++;
    }
    if (len & 1)
        ret += uint32_t(*(reinterpret_cast<uint8_t *>(pword)) << 8);

    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}