#ifndef ICMPHDR_H_
#define ICMPHDR_H_

#include "hpch.h"

#include "iphdr.h"

#define ICMP_SIZE 4

#pragma pack(push, 1)
struct IcmpHdr {
    uint8_t type_;
    uint8_t code_;
    uint16_t checksum_;
    uint16_t ident_;
    uint16_t seq_;

    uint8_t type() { return type_; }
    uint8_t code() { return code_; }
    uint16_t checksum() { return ntohs(checksum_); }
    uint16_t ident() { return ntohs(ident_); }
    uint16_t seq() { return ntohs(seq_); }

    void setUnused() { ident_ = seq_ = 0; }
    static uint16_t calcIcmpChecksum(IpHdr *, IcmpHdr *);
};
#pragma pack(pop)

#endif // ICMPHDR_H_
