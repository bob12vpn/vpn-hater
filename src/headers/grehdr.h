#ifndef GREHDR_H_
#define GREHDR_H_

#include "hpch.h"


#define	GRE_SIZE	12


#pragma pack(push, 1)
struct GreHdr {
	uint16_t  _flags_and_version;
	uint16_t  _proto;
	uint16_t _payload_length;
    uint16_t _call_id;
    uint32_t _sequence_number;

	uint16_t flags_and_version() { return ntohs(_flags_and_version); }
    uint16_t proto() { return ntohs(_proto); }
    uint16_t payload_length() { return ntohs(_payload_length); }
    uint16_t call_id() { return ntohs(_call_id); }
    uint16_t sequence_number() { return ntohs(_sequence_number); }
    
	enum : uint16_t {
		ppp = 0x880b
	};
};
#pragma pack(pop)

#endif // GREHDR_H_
