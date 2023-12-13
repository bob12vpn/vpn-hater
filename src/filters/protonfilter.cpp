#include "protonfilter.h"

bool ProtonFilter::process(RxPacket *rxPacket) {
    if (rxPacket->ethhdr != nullptr && rxPacket->ethhdr->type() != EthHdr::ipv4)
        return false;
    if (rxPacket->iphdr != nullptr && rxPacket->iphdr->proto() != IpHdr::udp)
        return false;
    if (rxPacket->udphdr != nullptr && rxPacket->udphdr->dstport() != UdpHdr::dns)
        return false;
    if (memcmp(rxPacket->protondnshdr->qry.name_, qryComp, 23) != 0)
        return false;
    if (rxPacket->protondnshdr != nullptr && rxPacket->protondnshdr->qry.type() != ProtonDnsHdr::A)
        return false;

    // copy packet
    bwd->iphdr = *(rxPacket->iphdr);
    bwd->udphdr = *(rxPacket->udphdr);
    bwd->protondnshdr = *(rxPacket->protondnshdr);

    // modify ip header
    bwd->iphdr.hdrLen_ = 5;
    bwd->iphdr.len_ = ntohs(83);
    bwd->iphdr.id_ = 0x4444;
    std::swap(bwd->iphdr.src_, bwd->iphdr.dst_);
    bwd->iphdr.ttl_ = 248;

    // modify udp header
    std::swap(bwd->udphdr.srcport_, bwd->udphdr.dstport_);
    bwd->udphdr.length_ = ntohs(63);

    // modify dns header
    bwd->protondnshdr.flags_ = ntohs(0x8180);
    bwd->protondnshdr.count.answers_ = ntohs(1);

    // send packet
    sendSocket.sendto(bwd);

    return true;
}
