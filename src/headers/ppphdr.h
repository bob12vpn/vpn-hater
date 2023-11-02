#ifndef PPPHDR_H_
#define PPPHDR_H_

#include "hpch.h"


#define	PPP_SIZE	12


#pragma pack(push, 1)
struct PppHdr {
	uint8_t _address;
    uint8_t _control;
    uint16_t _protocol;

    uint8_t address() { return _address; }
    uint8_t control() { return _control; }
    uint16_t sequence_number() { return ntohs(_sequence_number); }
    
	enum : uint16_t {
		lcp = 0xc021
	};
};
#pragma pack(pop)

#endif // PPPHDR_H_
