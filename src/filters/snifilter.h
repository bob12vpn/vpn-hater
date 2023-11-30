#ifndef SNIFILTER_H_
#define SNIFILTER_H_

#include "../pch.h"

#include "../flowkey.h"
#include "../packet.h"
#include "filter.h"

#define SNI_HIT_COUNT 1

class SniFilter : public Filter {
    TxTcpPacket *fwd{nullptr};
    TxTcpPacket *bwd{nullptr};

    bool isSniSetLoaded = false;

    FlowKey flowKey;
    std::map<FlowKey, uint32_t> flow;

public:
    SniFilter() {
        fwd = new TxTcpPacket;
        bwd = new TxTcpPacket;
    }

    std::unordered_set<std::string> sniSet;

    bool process(RxPacket *) override;
    bool loadSni(char *);
};

#endif // SNIFILTER_H_
