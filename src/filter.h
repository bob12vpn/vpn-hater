#ifndef FILTER_H_
#define FILTER_H_

#include "pch.h"

#include "packet.h"

bool isTcp(RxPacket *pkt);
bool isTcp(RxOpenVpnTcpPacket *pkt);

bool isOpenVpnTcp(RxOpenVpnTcpPacket *pkt);


#endif // FILTER_H_
