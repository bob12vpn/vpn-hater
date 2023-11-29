#ifndef OPENVPNTCPFILTER_H_
#define OPENVPNTCPFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class OpenVpnTcpFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxTcpPacket *fwd{nullptr};
    TxTcpPacket *bwd{nullptr};

public:
    OpenVpnTcpFilter() {
        fwd = new TxTcpPacket;
        bwd = new TxTcpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // OPENVPNTCPFILTER_H_