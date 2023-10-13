#ifndef HDR_H_
#define HDR_H_


#include <cstdint>
#include <string>
#include <netinet/in.h>


#define	ETH_SIZE	14
#pragma pack(push, 1)
struct _eth {
	uint8_t  dst[6];
	uint8_t  src[6];
	uint16_t type;
	
	std::string srcStr() {
		char tmp[17];
		sprintf(tmp, "%x:%x:%x:%x:%x:%x", src[0], src[1], src[2], src[3], src[4], src[5]);
		return std::string(tmp, tmp+17);
	}
	std::string dstStr() {
		char tmp[17];
		sprintf(tmp, "%x:%x:%x:%x:%x:%x", dst[0], dst[1], dst[2], dst[3], dst[4], dst[5]);
		return std::string(tmp, tmp+17);
	}
	enum : uint16_t {
		IPv4 = 0x0800
	};
};
#pragma pack(pop)


struct _ip {
	uint8_t hdr_len:4,
		version:4;
	uint8_t dsfield;
	uint16_t len;
	uint16_t id;
	uint16_t flags:3,
		 frag_offset:13;
	uint8_t ttl;
	uint8_t proto;
	uint16_t checksum;
	union {
		uint8_t srcmask[4];
		uint32_t src;
	};
	union {
		uint8_t dstmask[4];
		uint32_t dst;
	};

	uint16_t ip_size() {
		return (uint16_t)hdr_len * 4;
	}
	uint16_t calcIpChecksum(_ip *ip) {
		uint32_t ret = 0;
		ip->checksum = 0;
		for(int i=0; i<ip->ip_size(); i++) {
			ret += (*((uint8_t*)ip + i) << 8) + (*((uint8_t*)ip + i + 1));
		}
		ret += ret >> 16;
		return ntohs(~(uint16_t)ret);
	}
};
#define	IP_PROTO_TCP	6
#define	IP_PROTO_UDP	17


#define	UDP_SIZE	8
struct _udp {
	uint16_t srcport;
	uint16_t dstport;
	uint16_t length;
	uint16_t checksum;
};
#define	UDP_DSTPORT_DNS	53
#define UDP_DSTPORT_TLS 443


struct _tcp {
	uint16_t srcport;
	uint16_t dstport;
	uint32_t seq_raw;
	uint32_t ack_raw;
	uint8_t flags1:4,
		hdr_len:4;
	uint8_t flags2;
	uint16_t window_size_value;
	uint16_t checksum;
	uint16_t urgent_pointer;

	uint16_t tcp_size() {
		return (uint16_t)hdr_len * 4;
	}
	uint16_t calcTcpChecksum(_ip *ip, _tcp *tcp) {
		uint32_t ret = 0;
		tcp->checksum = 0;
		
		ret += (ntohs(ip->src >> 16)) + (ntohs(ip->src & 0xFFFF));
		ret += (ntohs(ip->dst >> 16)) + (ntohs(ip->dst & 0xFFFF));
		ret += ip->proto;
		ret += tcp->tcp_size();
		for(int i=0; i<tcp->tcp_size(); i+=2) {
			ret += (*((uint8_t*)tcp + i) << 8) + (*((uint8_t*)tcp + i + 1));
		}
		ret += ret >> 16;
		return ntohs(~(uint16_t)ret);
	}
};
#define	TCP_DSTPORT_DNS	53
#define TCP_DSTPORT_TLS 443
#define TCP_FLAGS_ACK 0x10
#define TCP_FLAGS_RSTACK 0x14


#define	DNS_SIZE	12
struct _dns {
	uint16_t id;
	uint16_t flags;
	struct count {
		uint16_t queries;
		uint16_t answers;
		uint16_t auth_rr;
		uint16_t add_rr;
	};
};


#define TLS_RECORD_SIZE 5
#define TLS_HANDSHAKE_RANDOM_SIZE 32
struct _extension {
	uint16_t type;
	uint16_t len;
	uint16_t server_name_list_len;
	uint8_t server_name_type;
	uint16_t server_name_len;
	std::string server_name;
};
struct _record {
	uint8_t content_type;
	uint16_t version;
	uint16_t length;
};
struct _handshake {
	uint8_t type;
	uint32_t length;
	uint16_t version;
	std::string random;
	uint8_t session_id_length;
	std::string session_id;
	uint16_t cipher_suites_length;
	std::string ciphersuites;
	uint8_t comp_methods_length;
	std::string comp_methods;
	uint16_t extensions_length;
	struct _extension extension;
};
struct _tls_client_hello {
	struct _record record;
	struct _handshake handshake;
};
#define TLS_RECORD_CONTENT_TYPE_HANDSHAKE 22
#define TLS_HANDSHAKE_TYPE_CLIENT_HELLO 1
#define TLS_HANDSHAKE_EXTENSION_TYPE_SERVER_NAME 0


#endif // HDR_H_
