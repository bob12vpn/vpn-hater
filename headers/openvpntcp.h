#include "hpdu.h"


#pragma pack(push, 1)
struct OpenVpnTcpHdr {
	uint16_t _plen;
	uint32_t _type;

	uint16_t plen() { return ntohs(_plen); }
	uint32_t type() { return _type & 0xFF; }
	uint32_t peerid() { return ntohl(_type) & 0xFFFFFF; }
};
#pragma pack(pop)
