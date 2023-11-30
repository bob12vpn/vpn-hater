#include "snifilter.h"

bool SniFilter::loadSni(char *sni_file_name) {
    std::ifstream sni_file(sni_file_name);
    if (!sni_file) {
        GTRACE("loading sni from '%s' is failed", sni_file_name);
        return false;
    }
    std::string line;
    while (std::getline(sni_file, line)) {
        sniSet.insert(line);
    }
    GTRACE("sni set is loaded");
    isSniSetLoaded = true;
    return true;
}

bool SniFilter::process(RxPacket *rxPacket) {
    if (!isSniSetLoaded)
        return false;
    if (rxPacket->ethhdr != nullptr && rxPacket->ethhdr->type() != EthHdr::ipv4)
        return false;
    if (rxPacket->iphdr != nullptr && rxPacket->iphdr->proto() != IpHdr::tcp)
        return false;
    if (rxPacket->tcphdr != nullptr && rxPacket->tcphdr->flags() != (TcpHdr::flagsPsh | TcpHdr::flagsAck))
        return false;

    flowKey.init(rxPacket);
    if (flow.find(flowKey) == flow.end()) {
        flow.insert({flowKey, 0});
    }

    if (flow[flowKey] == 0) {
        if (rxPacket->tlshdr != nullptr && rxPacket->tcphdr->dstport() != TcpHdr::tls)
            return false;
        if (sniSet.find(rxPacket->tlshdr->serverName()) == sniSet.end())
            return false;
    }

    if (++flow[flowKey] < SNI_HIT_COUNT)
        return false;

    // copy packet
    fwd->iphdr = bwd->iphdr = *(rxPacket->iphdr);
    fwd->tcphdr = bwd->tcphdr = *(rxPacket->tcphdr);
    fwd->tcphdr.hdrLen_ = bwd->tcphdr.hdrLen_ = 5;

    // modify ip header
    fwd->iphdr.len_ = bwd->iphdr.len_ = ntohs(40);
    fwd->iphdr.id_ = bwd->iphdr.id_ = 0x4444;
    std::swap(bwd->iphdr.src_, bwd->iphdr.dst_);
    bwd->iphdr.ttl_ = 128;

    // modify tcp header
    std::swap(bwd->tcphdr.srcport_, bwd->tcphdr.dstport_);
    fwd->tcphdr.seqRaw_ = ntohl(rxPacket->tcphdr->seqRaw() + TcpHdr::payloadLen(rxPacket->iphdr, rxPacket->tcphdr));
    bwd->tcphdr.seqRaw_ = rxPacket->tcphdr->ackRaw_;
    bwd->tcphdr.ackRaw_ = rxPacket->tcphdr->seqRaw_;
    fwd->tcphdr.flags_ = bwd->tcphdr.flags_ = TcpHdr::flagsRst | TcpHdr::flagsAck;

    // calculate ip and tcp checksum
    fwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(fwd->iphdr));
    bwd->iphdr.checksum_ = IpHdr::calcIpChecksum(&(bwd->iphdr));

    fwd->tcphdr.checksum_ = TcpHdr::calcTcpChecksum(&(fwd->iphdr), &(fwd->tcphdr));
    bwd->tcphdr.checksum_ = TcpHdr::calcTcpChecksum(&(bwd->iphdr), &(bwd->tcphdr));

    // send packet
    sendSocket.sendto(fwd);
    sendSocket.sendto(bwd);

    return true;
}
