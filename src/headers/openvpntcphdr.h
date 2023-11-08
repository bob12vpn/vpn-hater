#ifndef OPENVPNTCPHDR_H_
#define OPENVPNTCPHDR_H_

#include "hpch.h"


#define OPENVPNTCP_SIZE 6


#pragma pack(push, 1)
struct OpenVpnTcpHdr {
	uint16_t plen_;
	uint32_t peerid_:24,
		 type_:8;

	uint16_t plen() { return ntohs(plen_); }
	uint8_t type() { return (uint8_t)type_; }
	uint32_t peerid() { return ntohl(peerid_); }
	
	void clear();
};
#pragma pack(pop)

#endif // OPENVPNTCPHDR_H_
