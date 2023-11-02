#ifndef UTILITY_H_
#define UTILITY_H_

#include "pch.h"

#include "packet.h"
#include "gtrace.h"

pcap_t* open_pcap(char* interface);

uint8_t hex2int(char n);

bool load_sni(char* sni_file_name, std::unordered_set<std::string> &ret);

#endif // UTILITY_H_
