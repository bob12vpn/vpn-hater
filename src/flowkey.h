#include "pch.h"

struct FlowKey {
    uint32_t srcIp;
    uint32_t dstIp;
    uint16_t srcPort;
    uint16_t dstPort;

    bool operator<(const FlowKey &var) const {
        if (srcIp != var.srcIp) {
            return srcIp < var.srcIp;
        } else if (dstIp != var.dstIp) {
            return dstIp < var.dstIp;
        } else if (srcPort != var.srcPort) {
            return srcPort < var.srcPort;
        } else {
            return dstPort < var.dstPort;
        }
    }
};