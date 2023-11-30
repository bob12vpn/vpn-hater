#ifndef OPENVPNUDPHDR_H_
#define OPENVPNUDPHDR_H_

#include "hpch.h"

#define OPENVPNUDP_SIZE 4

#define ntohll(x) ((((uint64_t)ntohl(x)) << 32) + ntohl(x >> 32))

#pragma pack(push, 1)
struct OpenVpnUdpHdr {
    uint8_t type_;
    uint64_t sessionid_;
    uint8_t mpidarraylength_;
    uint32_t mpid_;

    uint8_t type() { return (uint8_t)type_; }
    uint64_t sessionid() { return ntohll(sessionid_); }
    uint8_t mpidarraylength() { return mpidarraylength_; }
    uint32_t mpid() { return ntohl(mpid_); }

    enum : uint32_t {
        P_CONTROL_HARD_RESET_CLIENT_V2 = 0x38,
        P_CONTROL_V1 = 0x20,
        P_ACK_V1 = 0x28
    };
};
#pragma pack(pop)

#endif // OPENVPNUDPHDR_H_
