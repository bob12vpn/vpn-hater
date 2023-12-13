#include "../pch.h"

#include "../packet.h"
#include "filter.h"
#include "l2tpfilter.h"
#include "openvpntcpfilter.h"
#include "pptpfilter.h"
#include "protonfilter.h"
#include "snifilter.h"
#include "tcpackfilter.h"

struct FilterManager : std::list<Filter *> {
    FilterManager();

    L2tpFilter l2tpFilter;
    OpenVpnTcpFilter openVpnTcpFilter;
    PptpFilter pptpFilter;
    ProtonFilter protonFilter;
    SniFilter sniFilter;
    // TcpAckFilter tcpAckFilter;

    bool openRawSockets(char *);
    bool processAll(RxPacket *);
};