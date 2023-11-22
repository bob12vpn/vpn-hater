#ifndef TCPACKFILTER_H_
#define TCPACKFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"

class TcpAckFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};
    
public:
    TcpAckFilter() {
        fwd = new TxPacket;
        bwd = new TxPacket;
    }
    
    bool process(RxPacket*) override;
};

#endif // TCPACKFILTER_H_