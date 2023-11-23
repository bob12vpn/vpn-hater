#include "../pch.h"

#include "../packet.h"
#include "filter.h"
#include "openvpntcpfilter.h"
#include "pptpfilter.h"
#include "snifilter.h"
#include "tcpackfilter.h"

struct FilterManager : std::list<Filter *> {
    FilterManager();

    OpenVpnTcpFilter openVpnTcpFilter;
    SniFilter sniFilter;
    PptpFilter pptpFilter;
    // TcpAckFilter tcpAckFilter;

    bool openRawSockets(char *);
    bool processAll(RxPacket *);
};