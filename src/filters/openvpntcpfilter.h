#ifndef OPENVPNTCPFILTER_H_
#define OPENVPNTCPFILTER_H_

#include "../pch.h"

#include "../flowkey.h"
#include "../packet.h"
#include "filter.h"

#define OPENVPNTCP_HIT_COUNT 5

class OpenVpnTcpFilter : public Filter {
    TxTcpPacket *fwd{nullptr};
    TxTcpPacket *bwd{nullptr};

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
    OpenVpnTcpFilter() {
        fwd = new TxTcpPacket;
        bwd = new TxTcpPacket;
    }

    bool process(RxPacket *) override;
};

#endif // OPENVPNTCPFILTER_H_