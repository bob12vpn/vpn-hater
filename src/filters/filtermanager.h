#include "../pch.h"

#include "filter.h"
#include "openvpntcpfilter.h"
#include "snifilter.h"
#include "pptpfilter.h"
#include "tcpackfilter.h"
#include "../packet.h"

struct FilterManager : std::list<Filter*> {
    FilterManager();
    
    OpenVpnTcpFilter openVpnTcpFilter;
	SniFilter sniFilter;
    PptpFilter pptpFilter;
    // TcpAckFilter tcpAckFilter;
    
    bool openRawSockets(char *);
    bool processAll(RxPacket*);
};