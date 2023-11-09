#include "tcpackfilter.h"

bool TcpAckFilter::parseAndFilter(const uint8_t *packet) override {
    this->rxPacket->parse(packet);
    if(this->rxPacket->ethhdr->type() != EthHdr::ipv4) return false;
	if(this->rxPacket->iphdr->proto() != IpHdr::tcp) return false;
	if(this->rxPacket->tcphdr->flags() != TcpHdr::flagsAck) return false;
    
    return true;
}

void TcpAckFilter::blocker(RawSock sendSocket) override {
    // copy packet
    fwd->iphdr  = bwd->iphdr  = *(rxPacket->iphdr);
    fwd->tcphdr = bwd->tcphdr = *(rxPacket->tcphdr);
    fwd->tcphdr.hdrLen_ = bwd->tcphdr.hdrLen_ = 5;

    // modify ip header
    fwd->iphdr.len_ = bwd->iphdr.len_ = ntohs(40);
    std::swap(bwd->iphdr.src_, bwd->iphdr.dst_);
    bwd->iphdr.ttl_ = 128;

    // modify tcp header
    std::swap(bwd->tcphdr.srcport_, bwd->tcphdr.dstport_);
    fwd->tcphdr.seqRaw_ = ntohl(rxPacket->tcphdr->seqRaw() + TcpHdr::payloadLen(rxPacket->iphdr, rxPacket->tcphdr));
    bwd->tcphdr.seqRaw_ = rxPacket->tcphdr->seqRaw_;
    fwd->tcphdr.flags_ = bwd->tcphdr.flags_ = TcpHdr::flagsRst | TcpHdr::flagsAck;

    // calculate ip and tcp checksum
    fwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(fwd->iphdr));
    bwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(bwd->iphdr));

    fwd->tcphdr.checksum_ = TcpHdr::calcTcpChecksum(&(fwd->iphdr), &(fwd->tcphdr));
    bwd->tcphdr.checksum_ = TcpHdr::calcTcpChecksum(&(bwd->iphdr), &(bwd->tcphdr));
    
    // send packet
    sendSocket.sendto(fwd);
    sendSocket.sendto(bwd);
}