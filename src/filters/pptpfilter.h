#ifndef PPTPFILTER_H_
#define PPTPFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"


class PptpFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    
public:
    PptpFilter() {
        fwd = new TxPacket;
    }
    
    bool process(RxPacket*) override;
};

#endif // PPTPFILTER_H_