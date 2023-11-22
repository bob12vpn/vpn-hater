#ifndef SNIFILTER_H_
#define SNIFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"

class SniFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};

public:
    SniFilter() {
        fwd = new TxPacket;
        bwd = new TxPacket;
    }
    
    std::unordered_set<std::string> sniSet;
    
    bool process(RxPacket*) override;
    bool loadSni(char*);
};

#endif // SNIFILTER_H_
