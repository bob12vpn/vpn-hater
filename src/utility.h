#ifndef UTILITY_H_
#define UTILITY_H_

#include "pch.h"

#include "packet.h"
#include "gtrace.h"

pcap_t* openPcap(char*);

uint8_t hex2int(char);

bool loadSni(char*, std::unordered_set<std::string>&);

void parsingPacket(RxPacket*, const uint8_t*);
void parsingPacket(RxOpenVpnTcpPacket*, const uint8_t*);

#endif // UTILITY_H_
