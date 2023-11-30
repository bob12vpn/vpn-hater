#include "flowkey.h"

void FlowKey::init(RxPacket *rxPacket) {
    ip1 = rxPacket->iphdr->src();
    ip2 = rxPacket->iphdr->dst();
    port1 = rxPacket->tcphdr->srcport();
    port2 = rxPacket->tcphdr->dstport();
    if (ip1 > ip2) {
        std::swap(ip1, ip2);
        std::swap(port1, port2);
    }
}