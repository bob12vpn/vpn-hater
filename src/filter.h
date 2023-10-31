#ifndef FILTER_H_
#define FILTER_H_

#include "pch.h"

#include "packet.h"

bool custom_filter(RxOpenVpnTcpPacket *pkt);

#endif // FILTER_H_