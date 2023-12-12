#ifndef PROTONDNSHDR_H_
#define PROTONDNSHDR_H_

#include "hpch.h"

#define PROTONDNS_SIZE 55

#pragma pack(push, 1)
struct ProtonDnsHdr {
    uint16_t id_;
    uint16_t flags_;
    struct Count {
        uint16_t queries_;
        uint16_t answers_;
        uint16_t authRr_;
        uint16_t addRr_;
    } count;
    struct Qry {
        uint8_t name_[23];
        uint16_t type_;
        uint16_t class_;

        uint16_t type() { return ntohs(type_); }
    } qry;
    struct Resp {
        uint8_t name_[2] = {0xc0, 0x0c};
        uint16_t type_ = ntohs(ProtonDnsHdr::A);
        uint16_t class_ = ntohs(0x0001);
        uint32_t ttl_ = ntohl(300);
        uint16_t len_ = ntohs(4);
        uint8_t a_[4] = {8, 8, 8, 8};
    } resp;

    enum : uint16_t {
        A = 1,
        AAAA = 28,
        HTTPS = 65
    };
};
#pragma pack(pop)

#endif // PROTONDNSHDR_H_
