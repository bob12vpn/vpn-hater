#ifndef SNIFILTER_H_
#define SNIFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class SniFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxTcpPacket *fwd{nullptr};
    TxTcpPacket *bwd{nullptr};

    bool isSetLoaded = false;

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
