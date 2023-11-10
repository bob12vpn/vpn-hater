#ifndef FILTER_H_
#define FILTER_H_

#include "../pch.h"

#include "../packet.h"

class Filter {
public:
    virtual void openRawSocket(char*) = 0;
    virtual bool filter(RxPacket*) = 0;
};

#endif // FILTER_H_
