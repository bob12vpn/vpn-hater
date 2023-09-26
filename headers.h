#ifndef HDR_H_
#define HDR_H_


#include <cstdint>
#include <string>


#define	ETH_SIZE	14
struct _eth {
	uint8_t  dst[6];
	uint8_t  src[6];
	uint16_t type;
};
#define	ETH_TYPE_IPV4	0x0800


#define	IP_SIZE	20
struct _ip {
	uint8_t version:4,
		hdr_len:4;
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
};
#define	TCP_DSTPORT_DNS	53
#define TCP_DSTPORT_TLS 443
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
