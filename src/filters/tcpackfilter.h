#ifndef TCPACKFILTER_H_
#define TCPACKFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"

class TcpAckFilter : public Filter {
    RxPacket *rxPacket;
    TxPacket *fwd;
	TxPacket *bwd;
    
public:
    TcpAckFilter() {
        rxPacket = new RxPacket;
        fwd = new TxPacket;
	    bwd = new TxPacket;
    }

    bool parseAndFilter(const uint8_t*) override;
    void blocker(RawSock) override;
};

#endif // TCPACKFILTER_H_