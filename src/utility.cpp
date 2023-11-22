#include "utility.h"

pcap_t* openPcap(char* interface) {
	char errbuf[PCAP_ERRBUF_SIZE];	
	pcap_t* pcap = pcap_open_live(interface, BUFSIZ, 1, -1, errbuf);
	if(pcap == NULL) {
		GTRACE("pcap_open_live(%s) return null - %s", interface, errbuf);
		return NULL;
	}
	GTRACE("pcap is opened");
	return pcap;
}

