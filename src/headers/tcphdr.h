#ifndef TCPHDR_H_
#define TCPHDR_H_

#include "hpch.h"

#include "iphdr.h"

#pragma pack(push, 1)
struct TcpHdr {
    uint16_t srcport_;
    uint16_t dstport_;
    uint32_t seqRaw_;
    uint32_t ackRaw_;
    uint8_t flagsReserved_ : 4,
        hdrLen_ : 4;
    uint8_t flags_;
    uint16_t windowSizeValue_;
    uint16_t checksum_;
    uint16_t urgentPointer_;

    uint16_t srcport() { return ntohs(srcport_); }
    uint16_t dstport() { return ntohs(dstport_); }
    uint32_t seqRaw() { return ntohl(seqRaw_); }
    uint32_t ackRaw() { return ntohl(ackRaw_); }
    uint8_t hdrLen() { return hdrLen_; }
    uint8_t flags() { return flags_; }
    uint16_t checksum() { return ntohs(checksum_); }

    uint16_t tcpHdrSize() { return (uint16_t)hdrLen() * 4; }
    static uint16_t payloadLen(IpHdr *, TcpHdr *);
    static uint16_t calcTcpChecksum(IpHdr *, TcpHdr *);

    enum : uint16_t {
        dns = 53,
        tls = 443
    };
    enum : uint8_t {
        flagsAck = 0x10,
        flagsPsh = 0x8,
        flagsRst = 0x4,
        flagsSyn = 0x2,
        flagsFin = 0x1
    };
};
#pragma pack(pop)

#endif // TCPHDR_H_
