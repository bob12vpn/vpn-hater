#ifndef FLOWKEY_H_
#define FLOWKEY_H_

#include "pch.h"

#include "packet.h"

typedef struct FlowKey {
    uint32_t ip1;
    uint32_t ip2;
    uint16_t port1;
    uint16_t port2;

    void init(RxPacket *);

    bool operator<(const FlowKey &var) const {
        if (ip1 != var.ip1) {
            return ip1 < var.ip1;
        } else if (ip2 != var.ip2) {
            return ip2 < var.ip2;
        } else if (port1 != var.port1) {
            return port1 < var.port1;
        } else {
            return port2 < var.port2;
        }
    }
} FlowKey;

#endif // FLOWKEY_H_