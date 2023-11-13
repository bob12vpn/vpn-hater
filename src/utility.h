#ifndef UTILITY_H_
#define UTILITY_H_

#include "pch.h"

#include "gtrace.h"

pcap_t* openPcap(char*);

bool loadSni(char*, std::unordered_set<std::string>&);

#endif // UTILITY_H_
