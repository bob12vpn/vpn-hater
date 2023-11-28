#ifndef L2TPFILTER_H_
#define L2TPFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"


class L2tpFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxL2tpPacket *fwd{nullptr};
    
public:
    L2tpFilter() {
        fwd = new TxL2tpPacket;
    }
    
    bool process(RxPacket*) override;
};

#endif // L2TPFILTER_H_
