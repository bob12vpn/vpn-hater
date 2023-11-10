#ifndef SNIFILTER_H_
#define SNIFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"
#include "../rawsock.h"

class SniFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};
    
    RawSock sendSocket;
    
public:
    void openRawSocket(char*) override;
    bool filter(RxPacket*) override;
};

#endif // SNIFILTER_H_