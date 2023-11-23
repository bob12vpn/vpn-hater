#ifndef ETHHDR_H_
#define ETHHDR_H_

#include "hpch.h"

#define ETH_SIZE 14

#pragma pack(push, 1)
struct EthHdr {
    uint8_t dst_[6];
    uint8_t src_[6];
    uint16_t type_;

    uint8_t *dst() { return dst_; }
    uint8_t *src() { return src_; }
    uint16_t type() { return ntohs(type_); }

    enum : uint16_t {
        ipv4 = 0x0800
    };
};
#pragma pack(pop)

#endif // ETHHDR_H_
