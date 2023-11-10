#ifndef PPPHDR_H_
#define PPPHDR_H_

#include "hpch.h"


#define	PPP_SIZE	4


#pragma pack(push, 1)
struct PppHdr {
	uint8_t address_;
    uint8_t control_;
    uint16_t protocol_;

    uint8_t address() { return address_; }
    uint8_t control() { return control_; }
    uint16_t protocol() { return ntohs(protocol_); }
    
	enum : uint16_t {
		lcp = 0xc021
	};
};
#pragma pack(pop)

#endif // PPPHDR_H_
