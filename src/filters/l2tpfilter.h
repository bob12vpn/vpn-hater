#ifndef L2TPFILTER_H_
#define L2TPFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class L2tpFilter : public Filter {
    TxL2tpPacket *fwd{nullptr};

public:
    L2tpFilter() {
        fwd = new TxL2tpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // L2TPFILTER_H_
