#ifndef TCPACKFILTER_H_
#define TCPACKFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class TcpAckFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxTcpPacket *fwd{nullptr};
    TxTcpPacket *bwd{nullptr};

public:
    TcpAckFilter() {
        fwd = new TxTcpPacket;
        bwd = new TxTcpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // TCPACKFILTER_H_