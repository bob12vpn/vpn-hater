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

uint8_t hex2int(char n) {
	return ('0' <= n && n <= '9') ? n - '0' : n - 'a' + 10;
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

void parsingPacket(RxPacket *dst, const uint8_t *src) {
	dst->ethhdr = (struct EthHdr*)(src);
	dst->iphdr = (struct IpHdr*)(src + ETH_SIZE);
	dst->tcphdr = (struct TcpHdr*)(src + ETH_SIZE + dst->iphdr->ipHdrSize());
}

void parsingPacket(RxOpenVpnTcpPacket *dst, const uint8_t *src) {
	dst->ethhdr = (struct EthHdr*)(src);
	dst->iphdr = (struct IpHdr*)(src + ETH_SIZE);
	dst->tcphdr = (struct TcpHdr*)(src + ETH_SIZE + dst->iphdr->ipHdrSize());
	dst->openvpntcphdr = (struct OpenVpnTcpHdr*)(src + ETH_SIZE + dst->iphdr->ipHdrSize() + dst->tcphdr->tcpHdrSize());
}
