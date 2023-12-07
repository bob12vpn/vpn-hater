#ifndef PPTPFILTER_H_
#define PPTPFILTER_H_

#include "../pch.h"

#include "../flowkey.h"
#include "../packet.h"
#include "filter.h"

#define PPTP_HIT_COUNT 5

class PptpFilter : public Filter {
    TxPptpPacket *fwd{nullptr};

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
    PptpFilter() {
        fwd = new TxPptpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // PPTPFILTER_H_