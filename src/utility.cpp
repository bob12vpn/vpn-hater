#include "utility.h"

pcap_t* open_pcap(char* interface) {
	char errbuf[PCAP_ERRBUF_SIZE];	
	pcap_t* pcap = pcap_open_live(interface, BUFSIZ, 1, -1, errbuf);
	if(pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", interface, errbuf);
		return NULL;
	}
	return pcap;
}

uint8_t hex2int(char n) {
	return ('0' <= n && n <= '9') ? n - '0' : n - 'a' + 10;
}
