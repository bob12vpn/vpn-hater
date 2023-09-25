// #define DEBUG

#include "headers.h"
#include "log.h"

// C++
#include <vector>
#include <string>

// C
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

void usage() {
	printf("usage: sudo ./drop-packet <interface>\n");
	printf("example: sudo ./drop-packet enp0s3\n");
}

uint16_t ntohs(uint16_t n) {
	return (n>>8) | (n<<8);
}

int main(int argc, char* argv[]) {
	LOG("Debug mode is enabled");
	if(argc != 2) {
		usage();
		return -1;
	}
	char* handle = argv[1];

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(handle, BUFSIZ, 1, 1000, errbuf);
	if(pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", handle, errbuf);
		return -1;
	}

	int cnt = 0;
	while(++cnt) {
		struct pcap_pkthdr* header;
		const uint8_t* packet;
		int res = pcap_next_ex(pcap, &header, &packet);
		if(res == 0) continue;

		struct _eth *eth = (struct _eth*)(packet);
		if(ntohs(eth->type) != ETH_TYPE_IPV4) continue;

		struct _ip *ip = (struct _ip*)(packet + ETH_SIZE);
		if(ip->proto != IP_PROTO_TCP && ip->proto != IP_PROTO_UDP) continue;

		struct _tcp *tcp = (struct _tcp*)(packet + ETH_SIZE + IP_SIZE);

		printf("========%d========\n", cnt);
	}
	
	return 0;
}
