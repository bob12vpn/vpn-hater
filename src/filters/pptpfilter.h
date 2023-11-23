#ifndef PPTPFILTER_H_
#define PPTPFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class PptpFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPptpPacket *fwd{nullptr};

public:
    PptpFilter() {
        fwd = new TxPptpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // PPTPFILTER_H_