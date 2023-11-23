#include "packet.h"

void RxPacket::parse(const uint8_t *pkt) {
    ethhdr = (struct EthHdr*)(pkt);
    switch(ethhdr->type()) {
    case EthHdr::ipv4:
        iphdr = (struct IpHdr*)(pkt + ETH_SIZE);
        switch(iphdr->proto()) {
        case IpHdr::tcp:
            tcphdr = (struct TcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize());
            if(tcphdr->payloadLen(iphdr, tcphdr) == 0) break;
            if(tcphdr->dstport() == TcpHdr::tls) {
                tlshdr->parse(pkt + ETH_SIZE + iphdr->ipHdrSize() + tcphdr->tcpHdrSize());
            }
            openvpntcphdr = (struct OpenVpnTcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + tcphdr->tcpHdrSize());
            break;
        case IpHdr::udp:
            udphdr = (struct UdpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize());
            openvpnudphdr = (struct OpenVpnUdpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE);
            l2tphdr = (struct L2tpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE);
            ppphdr = (struct PppHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE + L2TP_SIZE);
            if(ppphdr->protocol() == PppHdr::lcp) {
                lcphdr = (struct LcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + UDP_SIZE + L2TP_SIZE + PPP_SIZE);
            }
            break;
        case IpHdr::gre:
            grehdr = (struct GreHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize());
            if(grehdr->proto() == GreHdr::ppp) {
                ppphdr = (struct PppHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + grehdr->greHdrSize());
                if(ppphdr->protocol() == PppHdr::lcp) {
                    lcphdr = (struct LcpHdr*)(pkt + ETH_SIZE + iphdr->ipHdrSize() + grehdr->greHdrSize() + PPP_SIZE);
                }
            }   // grehdr
            break;
        }   // iphdr
        break;
    }
}