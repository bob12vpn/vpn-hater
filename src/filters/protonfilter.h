#ifndef PROTONFILTER_H_
#define PROTONFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class ProtonFilter : public Filter {
    TxProtonDnsPacket *bwd{nullptr};

public:
    ProtonFilter() {
        bwd = new TxProtonDnsPacket;
    }

    bool process(RxPacket *) override;
};

#endif // PROTONFILTER_H_