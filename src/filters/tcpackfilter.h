#ifndef TCPACKFILTER_H_
#define TCPACKFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"
#include "../rawsock.h"

class TcpAckFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};
    
    RawSock sendSocket;
    
public:
    TcpAckFilter() {
        fwd = new TxPacket;
        bwd = new TxPacket;
    }
    
    bool openRawSocket(char*) override;
    bool filter(RxPacket*) override;
};

#endif // TCPACKFILTER_H_