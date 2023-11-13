#include "pch.h"

#include "packet.h"
#include "utility.h"
#include "gtrace.h"
#include "rawsock.h"

#include "filters/filter.h"
#include "filters/openvpntcpfilter.h"
#include "filters/snifilter.h"
#include "filters/tcpackfilter.h"

void usage() {
	printf("usage: sudo ./block-packet <mirror interface> <send interface> [sni list]\n");
	printf("example: sudo ./block-packet enp0s3 eth0\n");
	printf("example: sudo ./block-packet enp0s3 eth0 sni.txt\n");
}

int main(int argc, char* argv[]) {
	gtrace_default("127.0.0.1", 8908, 1, "block-packet.log");
	GTRACE("Logging level : Message");

	if(argc < 3) {
		usage();
		return -1;
	}
	char *mirrorInterface = argv[1];
	char *sendInterface = argv[2];

	pcap_t *mirrorPcap = openPcap(mirrorInterface);
	if(mirrorPcap == NULL) return -1;
	
	OpenVpnTcpFilter openVpnTcpFilter;
	SniFilter sniFilter;
	// only for debug
	// TcpAckFilter tcpAckFilter;
	
	if(argc == 4) {
		bool success = loadSni(argv[3], sniFilter.sniSet);
		if(!success) {
			return -1;
		}
	}
	
	std::list<Filter*> filters;
	filters.push_back(&openVpnTcpFilter);
	filters.push_back(&sniFilter);
	// filters.push_back(&tcpAckFilter);
	for(Filter *filter : filters) {
		if(!filter->openRawSocket(sendInterface)) {
			return -1;
		}
	}
	
	int pktCnt = 0;
	RxPacket *rxPacket = new RxPacket;
	struct pcap_pkthdr *header;
	const uint8_t *packet;
	while(true) {
		int res = pcap_next_ex(mirrorPcap, &header, &packet);
		if(res == 0) {
			usleep(1);
			continue;
		}

		pktCnt++;
		
		rxPacket->clear();
		rxPacket->parse(packet);
		
		for(Filter *filter : filters) {
			if(filter->filter(rxPacket)) {
				break;
			}
		}
		
		GTRACE("========%d========", pktCnt);
	}

	delete rxPacket;
	pcap_close(mirrorPcap);
	
	return 0;
}
