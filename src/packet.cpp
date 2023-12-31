#include "packet.h"

bool RxPacket::openPcap(char *interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    mirrorPcap = pcap_open_live(interface, BUFSIZ, 1, -1, errbuf);
    if (mirrorPcap == NULL) {
        GTRACE("pcap_open_live(%s) return null - %s", interface, errbuf);
        return false;
    }
    GTRACE("pcap is opened");
    return true;
}

void RxPacket::parse() {
    ethhdr = (struct EthHdr *)(packet);
    switch (ethhdr->type()) {
    case EthHdr::ipv4:
        iphdr = (struct IpHdr *)(packet + ETH_SIZE);
        switch (iphdr->proto()) {
        case IpHdr::icmp:
            icmphdr = (struct IcmpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize());
            break;
        case IpHdr::tcp:
            tcphdr = (struct TcpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize());
            if (tcphdr->payloadLen(iphdr, tcphdr) == 0)
                break;
            openvpntcphdr = (struct OpenVpnTcpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + tcphdr->tcpHdrSize());
            break;
        case IpHdr::udp:
            udphdr = (struct UdpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize());
            openvpnudphdr = (struct OpenVpnUdpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE);
            l2tphdr = (struct L2tpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE);
            ppphdr = (struct PppHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE + L2TP_SIZE);
            if (ppphdr->protocol() == PppHdr::lcp) {
                lcphdr = (struct LcpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE + L2TP_SIZE + PPP_SIZE);
            }
            break;
        case IpHdr::gre:
            grehdr = (struct GreHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize());
            if (grehdr->proto() == GreHdr::ppp) {
                ppphdr = (struct PppHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + GRE_SIZE);
                if (ppphdr->protocol() == PppHdr::lcp) {
                    lcphdr = (struct LcpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + GRE_SIZE + PPP_SIZE);
                }
            }
            break;
        }
        break;
    }
}
