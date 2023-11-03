#ifndef UTILITY_H_
#define UTILITY_H_

#include "pch.h"

#include "packet.h"
#include "gtrace.h"

pcap_t* open_pcap(char*);

uint8_t hex2int(char);

bool load_sni(char*, std::unordered_set<std::string>&);

void parsing_packet(RxPacket*, const uint8_t*);
void parsing_packet(RxOpenVpnTcpPacket*, const uint8_t*);

#endif // UTILITY_H_
