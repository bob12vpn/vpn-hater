#ifndef FILTER_H_
#define FILTER_H_

#include "../pch.h"

#include "../packet.h"
#include "../rawsock.h"

class Filter {
public:
    virtual bool filter(RxPacket*) = 0;
};

#endif // FILTER_H_
