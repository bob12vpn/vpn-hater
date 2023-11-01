#ifndef PACKET_H_
#define PACKET_H_

#include "headers/ethhdr.h"
#include "headers/iphdr.h"
#include "headers/tcphdr.h"
#include "headers/udphdr.h"
#include "headers/openvpntcphdr.h"

#pragma pack(push, 1)
struct RxPacket {
    struct EthHdr *eth{nullptr};
    
    struct IpHdr *ip{nullptr};
    
    struct TcpHdr *tcp{nullptr};
    struct UdpHdr *udp{nullptr};
};

struct RxOpenVpnTcpPacket : RxPacket {
    struct OpenVpnTcpHdr *openvpntcp{nullptr};
};

struct TxPacket {
    // raw socket does not need ETH
    // struct EthHdr eth;
    struct IpHdr ip;
    struct TcpHdr tcp;
};
#pragma pack(pop)

#endif // PACKET_H_
