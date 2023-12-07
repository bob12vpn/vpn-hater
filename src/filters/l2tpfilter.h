#ifndef L2TPFILTER_H_
#define L2TPFILTER_H_

#include "../pch.h"

#include "../flowkey.h"
#include "../packet.h"
#include "filter.h"

#define L2TP_HIT_COUNT 5

class L2tpFilter : public Filter {
    TxL2tpPacket *fwd{nullptr};

    FlowKey flowKey;
    struct FlowValue {
        uint32_t resetCnt = 0;
        enum {
            unknown,
            allow,
            block
        } state;
    };
    std::map<FlowKey, FlowValue> flow;

public:
    L2tpFilter() {
        fwd = new TxL2tpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // L2TPFILTER_H_
