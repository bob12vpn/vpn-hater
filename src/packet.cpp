#include "packet.h"

void RxPacket::parse() {
    ethhdr = (struct EthHdr *)(packet);
    switch (ethhdr->type()) {
    case EthHdr::ipv4:
        iphdr = (struct IpHdr *)(packet + ETH_SIZE);
        switch (iphdr->proto()) {
        case IpHdr::tcp:
            tcphdr = (struct TcpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize());
            if (tcphdr->payloadLen(iphdr, tcphdr) == 0)
                break;
            if (tcphdr->dstport() == TcpHdr::tls) {
                tlshdr->parse(packet + ETH_SIZE + iphdr->ipHdrSize() + tcphdr->tcpHdrSize(), len());
            }
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
            grehdr->setSeqAck();
            if (grehdr->proto() == GreHdr::ppp) {
                ppphdr = (struct PppHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + grehdr->greHdrSize());
                if (ppphdr->protocol() == PppHdr::lcp) {
                    lcphdr = (struct LcpHdr *)(packet + ETH_SIZE + iphdr->ipHdrSize() + grehdr->greHdrSize() + PPP_SIZE);
                }
            } // grehdr
            break;
        } // iphdr
        break;
    }
}