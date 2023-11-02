#ifndef FILTER_H_
#define FILTER_H_

#include "pch.h"

#include "packet.h"

bool not_want_filter(RxOpenVpnTcpPacket *pkt);

#endif // FILTER_H_
