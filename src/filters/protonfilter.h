#ifndef PROTONFILTER_H_
#define PROTONFILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "filter.h"

class ProtonFilter : public Filter {
    TxProtonDnsPacket *bwd{nullptr};

    uint8_t qryComp[23] = {0x07, 0x61, 0x63, 0x63, 0x6f, 0x75, 0x6e, 0x74, 0x09, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x6e, 0x76, 0x70, 0x6e, 0x03, 0x63, 0x6f, 0x6d, 0x00};

public:
    ProtonFilter() {
        bwd = new TxProtonDnsPacket;
    }

    bool process(RxPacket *) override;
};

#endif // PROTONFILTER_H_