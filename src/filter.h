#ifndef FILTER_H_
#define FILTER_H_

#include "pch.h"

#include "packet.h"

bool isTcpAck(RxPacket*);
bool isTcpAck(RxOpenVpnTcpPacket*);

bool isOpenVpnTcp(RxOpenVpnTcpPacket*);


#endif // FILTER_H_
