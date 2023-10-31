#ifndef UTILITY_H_
#define UTILITY_H_

#include "pch.h"

#include "packet.h"
#include "gtrace.h"

pcap_t* open_pcap(char* interface); // separate class

int open_raw_socket(char* interface);   // separate class

void send_packet(int socket, struct sockaddr_in addr_in_, TxPacket *pkt);

uint8_t hex2int(char n);

uint8_t* resolve_mac(char* interface);

#endif // UTILITY_H_