#ifndef UTILITY_H_
#define UTILITY_H_

#include "pch.h"

#include "packet.h"
#include "gtrace.h"

#define VAR_NAME(x) #x

pcap_t* open_pcap(char* interface);

uint8_t hex2int(char n);

#endif // UTILITY_H_
