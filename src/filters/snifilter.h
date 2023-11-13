#ifndef SNIFILTER_H_
#define SNIFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"
#include "../rawsock.h"
#include "../utility.h"

class SniFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};
    
    RawSock sendSocket;

public:
    SniFilter() {
        fwd = new TxPacket;
        bwd = new TxPacket;
    }
    
    std::unordered_set<std::string> sniSet;
    
    bool openRawSocket(char*) override;
    bool filter(RxPacket*) override;
};

#endif // SNIFILTER_H_
