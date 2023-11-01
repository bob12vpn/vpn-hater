#ifndef PACKET_H_
#define PACKET_H_

#include "headers/ethhdr.h"
#include "headers/iphdr.h"
#include "headers/tcphdr.h"
#include "headers/udphdr.h"
#include "headers/openvpntcphdr.h"

#pragma pack(push, 1)
struct RxPacket {
    const struct EthHdr *eth{nullptr};
    
    const struct IpHdr *ip{nullptr};
    
    const struct TcpHdr *tcp{nullptr};
    const struct UdpHdr *udp{nullptr};
    
    RxPacket() {
        eth = new EthHdr;
        ip = new IpHdr;
        tcp = new TcpHdr;
        udp = new UdpHdr;
    }
    
    ~RxPacket() {
        delete eth;
        delete ip;
        delete tcp;
        delete udp;
    }
};

struct RxOpenVpnTcpPacket : RxPacket {
    const struct OpenVpnTcpHdr *openvpntcp{nullptr};
    
    RxOpenVpnTcpPacket() : RxPacket() {
        openvpntcp = new OpenVpnTcpHdr;
    }
    
    ~RxOpenVpnTcpPacket() {
        delete openvpntcp;
    }
};

struct TxPacket {
    // raw socket does not need ETH
    // struct EthHdr eth;
    struct IpHdr ip;
    struct TcpHdr tcp;
};
#pragma pack(pop)

#endif // PACKET_H_
