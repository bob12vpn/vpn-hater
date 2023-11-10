#ifndef PACKET_H_
#define PACKET_H_

#include "headers/ethhdr.h"

#include "headers/iphdr.h"
#include "headers/grehdr.h"

#include "headers/tcphdr.h"
#include "headers/udphdr.h"
#include "headers/ppphdr.h"

#include "headers/openvpntcphdr.h"
#include "headers/tlshdr.h"

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
    struct TlsHdr *tlshdr{nullptr};
    
    struct LcpHdr *lcphdr{nullptr};
    
    void clear() {
        ethhdr = nullptr;
        
        iphdr = nullptr;
        grehdr = nullptr;
        
        tcphdr = nullptr;
        udphdr = nullptr;
        ppphdr = nullptr;
        
        openvpntcphdr = nullptr;
        tlshdr = nullptr;
        
        lcphdr = nullptr;
    }
    
    void parse(const uint8_t *pkt) {
        ethhdr = (struct EthHdr*)(pkt);
        switch(ethhdr->type()) {
        case EthHdr::ipv4:
            iphdr = (struct IpHdr*)(pkt + ETH_SIZE);
            switch(iphdr->proto()) {
            case IpHdr::tcp:
                tcphdr = (struct TcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize());
                if(tcphdr->dstport() == TcpHdr::tls) {
                    tlshdr->parse(pkt + ETH_SIZE + iphdr->ipHdrSize() + tcphdr->tcpHdrSize());
                }
                openvpntcphdr = (struct OpenVpnTcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + tcphdr->tcpHdrSize());
                break;
            case IpHdr::udp:
                udphdr = (struct UdpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize());
                break;
            case IpHdr::gre:
                grehdr = (struct GreHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize());
                if(grehdr->proto() == GreHdr::ppp) {
                    ppphdr = (struct PppHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + grehdr->greHdrSize());
                    if(ppphdr->protocol() == PppHdr::lcp) {
                        lcphdr = (struct LcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + grehdr->greHdrSize() + PPP_SIZE());
                    }
                }   // grehdr
                break;
            }   // iphdr
            break;
        }
    }
};

struct TxPacket {
    struct IpHdr iphdr;
    struct TcpHdr tcphdr;
};
#pragma pack(pop)

#endif // PACKET_H_
