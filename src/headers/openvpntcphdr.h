#ifndef OPENVPNTCPHDR_H_
#define OPENVPNTCPHDR_H_

#include "hpch.h"


#pragma pack(push, 1)
struct OpenVpnTcpHdr {
	uint16_t _plen;
	uint32_t _peerid:24,
		 _type:8;

	uint16_t plen() { return ntohs(_plen); }
	uint8_t type() { return (uint8_t)_type; }
	uint32_t peerid() { return ntohl(_type); }
};
#pragma pack(pop)

#endif // OPENVPNTCPHDR_H_
