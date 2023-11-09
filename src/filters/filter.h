#ifndef FILTER_H_
#define FILTER_H_

#include "../pch.h"

#include "../packet.h"

class Filter {
public:
    virtual bool parseAndFilter(const uint8_t*) = 0;
    virtual void blocker(RawSock) = 0;
};

#endif // FILTER_H_
