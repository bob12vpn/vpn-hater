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
    struct EthHdr *ethhdr{nullptr};
    
    struct IpHdr *iphdr{nullptr};
    
    struct TcpHdr *tcphdr{nullptr};
    struct UdpHdr *udphdr{nullptr};
    
    struct GreHdr *grehdr{nullptr};
    
    RxPacket() {
        ethhdr = new EthHdr;
        iphdr = new IpHdr;
        tcphdr = new TcpHdr;
        udphdr = new UdpHdr;
        grehdr = new GreHdr;
    }
    
    virtual void parse(const uint8_t *pkt) {
        this->ethhdr = (struct EthHdr*)(pkt);
        this->iphdr = (struct IpHdr*)(pkt + ETH_SIZE);
	    this->tcphdr = (struct TcpHdr*)(pkt + ETH_SIZE + this->iphdr->ipHdrSize());
    }
};

struct RxOpenVpnTcpPacket : RxPacket {
    struct OpenVpnTcpHdr *openvpntcphdr{nullptr};
    
    RxOpenVpnTcpPacket() : RxPacket() {
        openvpntcphdr = new OpenVpnTcpHdr;
    }
    
    void parse(const uint8_t *pkt) override {
        this->ethhdr = (struct EthHdr*)(pkt);
        this->iphdr = (struct IpHdr*)(pkt + ETH_SIZE);
	    this->tcphdr = (struct TcpHdr*)(pkt + ETH_SIZE + this->iphdr->ipHdrSize());
        this->openvpntcphdr = (struct OpenVpnTcpHdr*)(pkt + ETH_SIZE + this->iphdr->ipHdrSize() + this->tcphdr->tcpHdrSize());
    }
};

struct RxPppPacket : RxPacket {
    struct PppHdr *ppphdr{nullptr};
    struct LcpHdr *lcphdr{nullptr};
    
    RxPppPacket() : RxPacket() {
        ppphdr = new PppHdr;
        lcphdr = new LcpHdr;
    }
    
    void parse(const uint8_t *pkt) override {
        // @todo
    }
};

struct TxPacket {
    // raw socket does not need ETH
    // struct EthHdr eth;
    struct IpHdr iphdr;
    struct TcpHdr tcphdr;
};
#pragma pack(pop)

#endif // PACKET_H_
