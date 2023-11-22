#ifndef OPENVPNTCPFILTER_H_
#define OPENVPNTCPFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"

class OpenVpnTcpFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};
    
public:
    OpenVpnTcpFilter() {
        fwd = new TxPacket;
        bwd = new TxPacket;
    }
    
    bool process(RxPacket*) override;
};

#endif // OPENVPNTCPFILTER_H_