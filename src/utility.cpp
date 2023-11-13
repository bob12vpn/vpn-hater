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

bool loadSni(char* sni_file_name, std::unordered_set<std::string> &ret) {
	std::ifstream sni_file(sni_file_name);
	if(!sni_file) {
		GTRACE("loading sni from '%s' is failed", sni_file_name);
		return false;
	}
	std::vector<std::string> sni_vec;
	std::string line;
	while(std::getline(sni_file, line)) {
		sni_vec.push_back(line);
	}
	ret = std::unordered_set<std::string>(sni_vec.begin(), sni_vec.end());
	return true;
}