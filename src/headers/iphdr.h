#ifndef IPHDR_H_
#define IPHDR_H_

#include "hpch.h"

#pragma pack(push, 1)
struct IpHdr {
    uint8_t hdrLen_ : 4,
        version_ : 4;
    uint8_t dsfield_;
    uint16_t len_;
    uint16_t id_;
    uint16_t flags_ : 3,
        fragOffset_ : 13;
    uint8_t ttl_;
    uint8_t proto_;
    uint16_t checksum_;
    union {
        uint8_t srcmask_[4];
        uint32_t src_;
    };
    union {
        uint8_t dstmask_[4];
        uint32_t dst_;
    };

    uint8_t hdrLen() { return hdrLen_; }
    uint8_t version() { return version_; }
    uint16_t len() { return ntohs(len_); }
    uint16_t id() { return ntohs(id_); }
    uint8_t proto() { return proto_; }
    uint16_t checksum() { return ntohs(checksum_); }
    uint32_t src() { return src_; } // little endian, for simple implementation
    uint32_t dst() { return dst_; } // little endian, same reason
    uint8_t *srcmask() { return srcmask_; }
    uint8_t *dstmask() { return dstmask_; }

    uint16_t ipHdrSize() { return (uint16_t)hdrLen() * 4; }
    static uint16_t calcIpChecksum(IpHdr *);

    enum : uint16_t {
        icmp = 1,
        tcp = 6,
        udp = 17,
        gre = 47
    };
};
#pragma pack(pop)

#endif // IPHDR_H_
