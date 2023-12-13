#ifndef RAWSOK_H_
#define RAWSOK_H_

#include "pch.h"

#include "gtrace.h"
#include "packet.h"

struct RawSock {
    int socket = 0;
    struct sockaddr_in addr_in_;

    bool open(char *);
    bool close();

    bool sendto(TxTcpPacket *);
    bool sendto(TxPptpPacket *);
    bool sendto(TxL2tpPacket *);
};

#endif // RAWSOK_H_
