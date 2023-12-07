#include "l2tpfilter.h"

bool L2tpFilter::process(RxPacket *rxPacket) {
    if (rxPacket->ethhdr != nullptr && rxPacket->ethhdr->type() != EthHdr::ipv4)
        return false;
    if (rxPacket->iphdr != nullptr && rxPacket->iphdr->proto() != IpHdr::udp)
        return false;
    if (rxPacket->iphdr->id() == 0x4444)
        return false;

    flowKey.init(rxPacket);
    if (flow.find(flowKey) == flow.end()) {
        flow.insert({flowKey, {0, FlowValue::unknown}});
    }

    if (flow[flowKey].state == FlowValue::allow) {
        return false;
    } else if (flow[flowKey].state == FlowValue::unknown) {
        if ((rxPacket->udphdr->dstport() != 1701 || rxPacket->udphdr->srcport() != 1701)) {
            flow[flowKey].state = FlowValue::allow;
            return false;
        }
    }

    if (++flow[flowKey].resetCnt < L2TP_HIT_COUNT) {
        return false;
    } else {
        flow[flowKey].state = FlowValue::block;
    }

    // copy packet
    fwd->iphdr = *(rxPacket->iphdr);
    fwd->udphdr = *(rxPacket->udphdr);
    fwd->l2tphdr = *(rxPacket->l2tphdr);

    // modify ip header
    fwd->iphdr.id_ = 0x4444;
    fwd->iphdr.flags_ = 0x000;
    fwd->iphdr.ttl_ = 128;

    // modify udp header

    // modify l2tp header
    fwd->l2tphdr.flags_ = htons(0x0002);
    // fwd->l2tphdr.tunnel_ =
    // fwd->l2tphdr.session_ =

    // modify ppp header
    fwd->ppphdr.address_ = 0xFF;
    fwd->ppphdr.control_ = 0x03;
    fwd->ppphdr.protocol_ = htons(PppHdr::lcp);

    // LCP header setup
    fwd->lcphdr.code_ = 0x05;
    fwd->lcphdr.identifier_ = 0x01;

    // setup lcpdhr length
    fwd->lcphdr.length_ = htons(LCP_SIZE);
    // setup udphdr length
    fwd->udphdr.length_ = htons(UDP_SIZE + L2TP_SIZE + PPP_SIZE + LCP_SIZE);
    // setup iphdr length
    uint16_t ipTotalLength = fwd->iphdr.ipHdrSize() + fwd->udphdr.length(); // Need to add udpHdrSize
    fwd->iphdr.len_ = htons(ipTotalLength);

    // calculate ip and tcp checksum
    fwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(fwd->iphdr));

    // send packet
    sendSocket.sendto(fwd);

    return true;
}
