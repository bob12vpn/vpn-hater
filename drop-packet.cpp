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
	printf("usage: sudo ./tls-hello <interface> [option]\n");
	printf("option: \n\t--debug\tprint logs\n");
	printf("example: sudo ./tls-helle enp0s3\n");
}

uint16_t ntohs(uint16_t n) {
	return (n>>8) | (n<<8);
}

uint16_t read16b(const uint8_t *c) {
	return (uint16_t)(c[0] << 8 | c[1]);
}
uint32_t read24b(const uint8_t *c) {
	return (uint32_t)(c[1] << 16 | c[2] << 8 | c[3]);
}

int dump_client_hello(struct _tls_client_hello *tls, const uint8_t* packet) {
	int offset = 0;
	int id_len, ci_len, cm_len;
	
	tls->record.content_type = (uint8_t)packet[offset];
		offset += 1;
	if(tls->record.content_type != TLS_RECORD_CONTENT_TYPE_HANDSHAKE) return -1;

	LOG("Handshake");

	tls->record.version = read16b(packet + offset);
		offset += 2;
	tls->record.length = read16b(packet + offset);
		offset += 2;
	tls->handshake.type = (uint8_t)packet[offset];
		offset += 0;
	if(tls->handshake.type != TLS_HANDSHAKE_TYPE_CLIENT_HELLO) return -2;

	LOG("Client hello");

	tls->handshake.length = read24b(packet + offset);
		offset += 4;
	tls->handshake.version = read16b(packet + offset);
		offset += 2;
	tls->handshake.random = std::string(packet + offset, packet + offset + TLS_HANDSHAKE_RANDOM_SIZE);
		offset += 32;
	tls->handshake.session_id_length = id_len = (uint8_t)packet[offset];
		offset += 1;
	tls->handshake.session_id = std::string(packet + offset, packet + offset + id_len);
		offset += id_len;
	tls->handshake.cipher_suites_length = ci_len = read16b(packet + offset);
		offset += 2;
	tls->handshake.ciphersuites = std::string(packet + offset, packet + offset + ci_len);
		offset += ci_len;
	tls->handshake.comp_methods_length = cm_len = (uint8_t)packet[offset];
		offset += 1;
	LOG("BREAK1");
	PRINT(offset);
	PRINT(cm_len);
	PRINT(packet[offset+1]);
	tls->handshake.comp_methods = std::string(packet + offset, packet + offset + cm_len);
		offset += cm_len;
	LOG("BREAK2");
	tls->handshake.extensions_length = read16b(packet + offset);
		offset += 2;

	uint16_t type, len, name_len;
	while(true) {
		type = read16b(packet + offset);
			offset += 2;
		len = read16b(packet + offset);
			offset += 2;
		if(type == TLS_HANDSHAKE_EXTENSION_TYPE_SERVER_NAME) {
			tls->handshake.extension.server_name_list_len = read16b(packet + offset);
				offset += 2;
			tls->handshake.extension.server_name_type = (uint8_t)packet[offset];
				offset += 1;
			tls->handshake.extension.server_name_len = name_len = read16b(packet + offset);
				offset += 2;
			PRINT(tls->handshake.extension.server_name_len);
			tls->handshake.extension.server_name = std::string(packet + offset, packet + offset + name_len);

			LOG("Server name is found");
			return 0;
		}
		offset += len;
	}
}

int main(int argc, char* argv[]) {
#ifdef DEBUG
	LOG("Debug mode is enabled");
#endif	// DEBUG
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

		struct _tcp *tcp;
		struct _udp *udp;
		if(ip->proto == IP_PROTO_TCP) {
			tcp = (struct _tcp*)(packet + ETH_SIZE + IP_SIZE);
			if(ntohs(tcp->dstport) != TCP_DSTPORT_TLS) continue;
			if(ntohs(ip->len) - IP_SIZE - tcp->hdr_len*4 == 0) continue;	// for filter handshake with garbage value
		}
		else if(ip->proto == IP_PROTO_UDP) {
			// @todo: UDP's "Client Hello"'s proto is QUIC
			continue;
		}
		else {
			continue;
		}

		struct _tls_client_hello *tls = new _tls_client_hello;
		int offset = ETH_SIZE + IP_SIZE + (ip->proto == IP_PROTO_TCP ? tcp->hdr_len*4 : UDP_SIZE);
		res = dump_client_hello(tls, packet + offset);
		if(res < 0) {
			delete tls;
			continue;
		}

		LOG("This is Client Hello");

		printf("%s\n", tls->handshake.extension.server_name.c_str());
		printf("========%d========\n", cnt);
	}
	
	return 0;
}
