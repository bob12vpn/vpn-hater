#include "pptpfilter.h"

bool PptpFilter::process(RxPacket *rxPacket) {
    if(rxPacket->ethhdr != nullptr && rxPacket->ethhdr->type() != EthHdr::ipv4) return false;
	if(rxPacket->iphdr != nullptr && rxPacket->iphdr->proto() != IpHdr::gre) return false;

    // copy packet
    fwd->iphdr  = *(rxPacket->iphdr);
    fwd->grehdr = *(rxPacket->grehdr);
    fwd->ppphdr = *(rxPacket->ppphdr);
    fwd->lcphdr = *(rxPacket->lcphdr);

    // modify ip header
    fwd->iphdr.len_ = ntohs(20); // We have to check 
    fwd->iphdr.id_ = 0x4444;
    fwd->iphdr.flags_ = 0x000;
    fwd->iphdr.ttl_ = 128;

    // modify gre header

    fwd->grehdr.proto_ = htons(GreHdr::ppp);
    fwd->grehdr.sequenceNumber_ += ntohl(1);
    
    //modify PPP header
    fwd->ppphdr.address_ = 0xFF;
    fwd->ppphdr.control_ = 0x03;
    fwd->ppphdr.protocol_ = htons(PppHdr::lcp);

    //LCP header setup
    fwd->lcphdr.code_ = 0x05;
    fwd->lcphdr.identifier_ = 0x01;
    uint8_t lcpData[] = {0x7a, 0xb4, 0x59, 0xc1, 0x00, 0x3c, 0xcd, 0x74, 0x00, 0x00, 0x00, 0x00};//temp


    //setup lcpdhr length
    fwd->lcphdr.length_ = htons(sizeof(LcpHdr) + sizeof(lcpData));
    //setup grehdr length
    fwd->grehdr.payloadLength_ = htons(sizeof(PppHdr) + ntohs(fwd->lcphdr.length_));
    //setup iphdr length
    size_t ipTotalLength = sizeof(IpHdr) + sizeof(GreHdr) + ntohs(fwd->grehdr.payloadLength_);
    fwd->iphdr.len_ = htons(ipTotalLength);

    // calculate ip and tcp checksum
    fwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(fwd->iphdr));
    
    // send packet
    sendSocket.sendto(fwd);
    
    return true;
}