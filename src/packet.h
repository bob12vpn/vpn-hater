#ifndef PACKET_H_
#define PACKET_H_

#include "headers/ethhdr.h"
#include "headers/iphdr.h"
#include "headers/tcphdr.h"
#include "headers/udphdr.h"
#include "headers/grehdr.h"
#include "headers/ppphdr.h"
#include "headers/lcphdr.h"
#include "headers/openvpntcphdr.h"

#pragma pack(push, 1)
struct RxPacket {
    struct EthHdr *eth{nullptr};
    
    struct IpHdr *ip{nullptr};
    
    struct TcpHdr *tcp{nullptr};
    struct UdpHdr *udp{nullptr};
    
    struct GreHdr *gre{nullptr};
    
    RxPacket() {
        eth = new EthHdr;
        ip = new IpHdr;
        tcp = new TcpHdr;
        udp = new UdpHdr;
        gre = new GreHdr;
    }
    
    ~RxPacket() {
        delete eth;
        delete ip;
        delete tcp;
        delete udp;
        delete gre;
    }
};

struct RxOpenVpnTcpPacket : RxPacket {
    struct OpenVpnTcpHdr *openvpntcp{nullptr};
    
    RxOpenVpnTcpPacket() : RxPacket() {
        openvpntcp = new OpenVpnTcpHdr;
    }
    
    ~RxOpenVpnTcpPacket() {
        delete openvpntcp;
    }
};

struct RxPppPacket : RxPacket {
    struct PppHdr *ppp{nullptr};
    struct LcpHdr *lcp{nullptr};
    
    RxPppPacket() : RxPacket() {
        ppp = new PppHdr;
        lcp = new LcpHdr;
    }
    
    ~RxPppPacket() {
        delete ppp;
        delete lcp;
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
