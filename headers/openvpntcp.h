#include "hpdu.h"


#pragma pack(push, 1)
struct OpenVpnTcpHdr {
	uint16_t _plen;
	uint32_t _type:8,
		 _peerid:24;

	uint16_t plen() { return ntohs(_plen); }
	uint32_t type() { return _type; }
	uint32_t peerid() { return ntohl(_peerid); }
};
#pragma pack(pop)