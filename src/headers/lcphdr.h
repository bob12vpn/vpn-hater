#ifndef LCPHDR_H_
#define LCPHDR_H_

#include "hpch.h"


#define	LCP_SIZE	4


#pragma pack(push, 1)
struct LcpHdr {
	uint8_t code_;
    uint8_t identifier_;
    uint16_t length_;

    uint8_t code() { return code_; }
    uint8_t identifier() { return identifier_; }
    uint16_t length() { return ntohs(length_); }
};
#pragma pack(pop)

#endif // LCPHDR_H_
