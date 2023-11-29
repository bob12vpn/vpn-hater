#ifndef PACKET_H_
#define PACKET_H_

#include "pch.h"

#include "headers/ethhdr.h"

#include "headers/grehdr.h"
#include "headers/iphdr.h"

#include "headers/ppphdr.h"
#include "headers/tcphdr.h"
#include "headers/udphdr.h"

#include "headers/l2tphdr.h"
#include "headers/openvpntcphdr.h"
#include "headers/openvpnudphdr.h"
#include "headers/tlshdr.h"

#include "headers/lcphdr.h"

#include "gtrace.h"

#pragma pack(push, 1)
struct RxPacket {
    RxPacket() {
        tlshdr = new TlsHdr;
    }

    pcap_t *mirrorPcap;
    struct pcap_pkthdr *header;
    const uint8_t *packet;

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

    void clear() {
        ethhdr = nullptr;

        iphdr = nullptr;
        grehdr = nullptr;

        tcphdr = nullptr;
        udphdr = nullptr;
        ppphdr = nullptr;

        openvpntcphdr = nullptr;
        openvpnudphdr = nullptr;
        l2tphdr = nullptr;

        lcphdr = nullptr;
    }

    bool openPcap(char *interface) {
        char errbuf[PCAP_ERRBUF_SIZE];
        mirrorPcap = pcap_open_live(interface, BUFSIZ, 1, -1, errbuf);
        if (mirrorPcap == NULL) {
            GTRACE("pcap_open_live(%s) return null - %s", interface, errbuf);
            return false;
        }
        GTRACE("pcap is opened");
        return true;
    }

    bool capture() {
        return pcap_next_ex(mirrorPcap, &header, &packet) != 0;
    }

    uint32_t len() {
        return (uint32_t)header->caplen;
    }

    void parse();
};

struct TxTcpPacket {
    struct IpHdr iphdr;
    struct TcpHdr tcphdr;
};

struct TxPptpPacket {
    struct IpHdr iphdr;
    struct GreHdr grehdr;
    struct PppHdr ppphdr;
    struct LcpHdr lcphdr;
};

struct TxL2tpPacket {
    struct IpHdr iphdr;
    struct UdpHdr udphdr;
    struct L2tpHdr l2tphdr;
    struct PppHdr ppphdr;
    struct LcpHdr lcphdr;
};

#pragma pack(pop)

#endif // PACKET_H_
