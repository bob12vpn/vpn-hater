#ifndef PACKET_H_
#define PACKET_H_

#include "headers/ethhdr.h"

#include "headers/iphdr.h"
#include "headers/grehdr.h"

#include "headers/tcphdr.h"
#include "headers/udphdr.h"
#include "headers/ppphdr.h"

#include "headers/openvpntcphdr.h"
#include "headers/openvpnudphdr.h"
#include "headers/tlshdr.h"
#include "headers/l2tphdr.h"

#include "headers/lcphdr.h"

#pragma pack(push, 1)
struct RxPacket {
    struct EthHdr *ethhdr{nullptr};
    
    struct IpHdr *iphdr{nullptr};
    struct GreHdr *grehdr{nullptr};
    
    struct TcpHdr *tcphdr{nullptr};
    struct UdpHdr *udphdr{nullptr};
    struct PppHdr *ppphdr{nullptr};
    
    struct OpenVpnTcpHdr *openvpntcphdr{nullptr};
    struct OpenVpnUdpHdr *openvpnudphdr{nullptr};
    struct TlsHdr *tlshdr{nullptr};
    struct L2tpHdr *l2tphdr{nullptr};
    
    struct LcpHdr *lcphdr{nullptr};
    
    RxPacket() {
        tlshdr = new TlsHdr;
    }
    
    void clear() {
        ethhdr = nullptr;
        
        iphdr = nullptr;
        grehdr = nullptr;
        
        tcphdr = nullptr;
        udphdr = nullptr;
        ppphdr = nullptr;
        
        openvpntcphdr = nullptr;
        
        lcphdr = nullptr;
    }
    
    void parse(const uint8_t *pkt);
};

struct TxPacket {
    struct IpHdr iphdr;
    struct TcpHdr tcphdr;
};

struct TxPptpPacket : TxPacket {
    struct IpHdr iphdr;
    struct GreHdr grehdr;
    struct PppHdr ppphdr;
    struct LcpHdr lcphdr;
};

#pragma pack(pop)


#endif // PACKET_H_
