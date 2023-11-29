#ifndef UDPHDR_H_
#define UDPHDR_H_

#include "hpch.h"

#include "iphdr.h"

#define UDP_SIZE 8

#pragma pack(push, 1)
struct UdpHdr {
    uint16_t srcport_;
    uint16_t dstport_;
    uint16_t length_;
    uint16_t checksum_;

    uint16_t srcport() { return ntohs(srcport_); }
    uint16_t dstport() { return ntohs(dstport_); }
    uint16_t length() { return ntohs(length_); }
    uint16_t checksum() { return ntohs(checksum_); }

    uint16_t payloadLen() { return length() - UDP_SIZE; }
    static uint16_t calcUdpChecksum(IpHdr *, UdpHdr *);

    enum : uint16_t {
        dns = 53,
        tls = 443
    };
};
#pragma pack(pop)

#endif // UDPHDR_H_
