#ifndef SNIFILTER_H_
#define SNIFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class SniFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};

    bool isSetLoaded = false;

public:
    SniFilter() {
        fwd = new TxPacket;
        bwd = new TxPacket;
    }

    std::unordered_set<std::string> sniSet;

    bool process(RxPacket *) override;
    bool loadSni(char *);
};

#endif // SNIFILTER_H_
