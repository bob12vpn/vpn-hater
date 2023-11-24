#include "pptpfilter.h"

bool PptpFilter::process(RxPacket *rxPacket) {
    if (rxPacket->ethhdr != nullptr && rxPacket->ethhdr->type() != EthHdr::ipv4)
        return false;
    if (rxPacket->iphdr != nullptr && rxPacket->iphdr->proto() != IpHdr::gre)
        return false;
    if (rxPacket->iphdr->id() == 0x4444)
        return false;

    // copy packet
    fwd->iphdr = *(rxPacket->iphdr);
    fwd->grehdr = *(rxPacket->grehdr);

    // modify ip header
    fwd->iphdr.id_ = 0x4444;
    fwd->iphdr.flags_ = 0x000;
    fwd->iphdr.ttl_ = 128;

    // modify gre header

    fwd->grehdr.proto_ = htons(GreHdr::ppp);
    fwd->grehdr.sequenceNumber_ += ntohl(1);

    // modify PPP header
    fwd->ppphdr.address_ = 0xFF;
    fwd->ppphdr.control_ = 0x03;
    fwd->ppphdr.protocol_ = htons(PppHdr::lcp);

    // LCP header setup
    fwd->lcphdr.code_ = 0x05;
    fwd->lcphdr.identifier_ = 0x01;

    // setup lcpdhr length
    fwd->lcphdr.length_ = htons(sizeof(fwd->lcphdr));
    // setup grehdr length
    fwd->grehdr.payloadLength_ = htons(sizeof(fwd->ppphdr) + ntohs(fwd->lcphdr.length_));
    // setup iphdr length
    uint16_t ipTotalLength = sizeof(fwd->iphdr) + sizeof(fwd->grehdr) + ntohs(fwd->grehdr.payloadLength_);
    fwd->iphdr.len_ = htons(ipTotalLength);

    // calculate ip and tcp checksum
    fwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(fwd->iphdr));

    // send packet
    sendSocket.sendto(fwd);

    return true;
}
