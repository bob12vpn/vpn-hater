#ifndef L2TPHDR_H_
#define L2TPHDR_H_

#include "hpch.h"


#define	L2TP_SIZE	6


#pragma pack(push, 1)
struct L2tpHdr {
	uint16_t flags_;
    uint16_t tunnel_;
    uint16_t session_;

    uint16_t flags() { return ntohs(flags_); }
    uint16_t tunnel() { return ntohs(tunnel_); }
    uint16_t session() { return ntohs(session_); }
};
#pragma pack(pop)

#endif // L2TPHDR_H_
