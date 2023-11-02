#ifndef LCPHDR_H_
#define LCPHDR_H_

#include "hpch.h"


#define	LCP_SIZE	4


#pragma pack(push, 1)
struct LcpHdr {
	uint8_t _code;
    uint8_t _identifier;
    uint16_t _length;

    uint8_t code() { return _code; }
    uint8_t identifier() { return _identifier; }
    uint16_t length() { return ntohs(_length); }
};
#pragma pack(pop)

#endif // LCPHDR_H_
