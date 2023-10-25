#include "hpdu.h"


#define	ETH_SIZE	14


#pragma pack(push, 1)
struct EthHdr {
	uint8_t  _dst[6];
	uint8_t  _src[6];
	uint16_t _type;

	uint8_t* dst() { return _dst; }
	uint8_t* src() { return _src; }
	uint16_t type() { return ntohs(_type); }
    
	enum : uint16_t {
		ipv4 = 0x0800
	};
};
#pragma pack(pop)