#ifndef OPENVPNTCPFILTER_H_
#define OPENVPNTCPFILTER_H_

#include "../pch.h"

#include "filter.h"
#include "../packet.h"
#include "../rawsock.h"

class OpenVpnTcpFilter : public Filter {
    RxPacket *rxPacket{nullptr};
    TxPacket *fwd{nullptr};
    TxPacket *bwd{nullptr};
    
    RawSock sendSocket;
    
public:
    void openRawSocket(char*) override;
    bool filter(RxPacket*) override;
};

#endif // OPENVPNTCPFILTER_H_