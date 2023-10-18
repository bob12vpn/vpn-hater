#ifndef HDR_H_
#define HDR_H_


#include <cstdint>
#include <string>
#include <netinet/in.h>


#define	ETH_SIZE	14
#pragma pack(push, 1)
struct _eth {
	uint8_t  _dst[6];
	uint8_t  _src[6];
	uint16_t _type;

	uint8_t* dst() { return _dst; }
	uint8_t* src() { return _src; }
	uint16_t type() { return ntohs(_type); }

	std::string srcStr() {
		char tmp[17];
		sprintf(tmp, "%x:%x:%x:%x:%x:%x", _src[0], _src[1], _src[2], _src[3], _src[4], _src[5]);
		return std::string(tmp, tmp+17);
	}
	std::string dstStr() {
		char tmp[17];
		sprintf(tmp, "%x:%x:%x:%x:%x:%x", _dst[0], _dst[1], _dst[2], _dst[3], _dst[4], _dst[5]);
		return std::string(tmp, tmp+17);
	}
	enum : uint16_t {
		ipv4 = 0x0800
	};
};
#pragma pack(pop)


struct _ip {
	uint8_t _hdr_len:4,
		_version:4;
	uint8_t _dsfield;
	uint16_t _len;
	uint16_t _id;
	uint16_t _flags:3,
		 _frag_offset:13;
	uint8_t _ttl;
	uint8_t _proto;
	uint16_t _checksum;
	union {
		uint8_t _srcmask[4];
		uint32_t _src;
	};
	union {
		uint8_t _dstmask[4];
		uint32_t _dst;
	};

	uint8_t hdr_len() { return _hdr_len; }
	uint8_t version() { return _version; }
	uint16_t dsfield() { return ntohs(_dsfield); }
	uint16_t len() { return ntohs(_len); }
	uint16_t id() { return ntohs(_id); }
	uint16_t flags() { return ntohs(_flags); }	// maybe unsafe
	uint16_t frag_offset() { return ntohs(_frag_offset); }	// maybe unsafe
	uint8_t ttl() { return _ttl; }
	uint8_t proto() { return _proto; }
	uint16_t checksum() { return ntohs(_checksum); }
	uint32_t src() { return _src; }	// big endian, for simple implementation
	uint32_t dst() { return _dst; }	// big endia, same reason

	uint16_t ip_size() {
		return (uint16_t)_hdr_len * 4;
	}
	static uint16_t calcIpChecksum(_ip *ip) {
		uint32_t ret = 0;
		ip->_checksum = 0;
		for(int i=0; i<ip->ip_size(); i+=2) {
			ret += (*((uint8_t*)ip + i) << 8) + (*((uint8_t*)ip + i + 1));
		}
		ret += ret >> 16;
		return ntohs(~(uint16_t)ret);
	}
	enum : uint16_t {
		tcp = 6,
		udp = 17
	};
};


#define	UDP_SIZE	8
struct _udp {
	uint16_t _srcport;
	uint16_t _dstport;
	uint16_t _length;
	uint16_t _checksum;

	uint16_t srcport() { return ntohs(_srcport); }
	uint16_t dstport() { return ntohs(_dstport); }
	uint16_t length() { return ntohs(_length); }
	uint16_t checksum() { return ntohs(_checksum); }
	
	enum : uint16_t {
		dns = 53,
		tls = 443
	};
};


struct _tcp {
	uint16_t _srcport;
	uint16_t _dstport;
	uint32_t _seq_raw;
	uint32_t _ack_raw;
	uint8_t _flags1:4,
		_hdr_len:4;
	uint8_t _flags2;
	uint16_t _window_size_value;
	uint16_t _checksum;
	uint16_t _urgent_pointer;

	uint16_t srcport() { return ntohs(_srcport); }
	uint16_t dstport() { return ntohs(_dstport); }
	uint32_t seq_raw() { return ntohl(_seq_raw); }
	uint32_t ack_raw() { return ntohl(_ack_raw); }
	uint8_t flags1() { return _flags1; }	// maybe unsafe
	uint8_t hdr_len() { return _hdr_len; }	// maybe unsafe
	uint8_t flags2() { return _flags2; }
	uint16_t window_size_value() { return ntohs(_window_size_value); }
	uint16_t checksum() { return ntohs(_checksum); }
	uint16_t urgent_pointer() { return ntohs(_urgent_pointer); }

	uint16_t tcp_size() {
		return (uint16_t)_hdr_len * 4;
	}
	static uint16_t len(_ip *ip, _tcp *tcp) {
		return ip->len() - ip->ip_size() - tcp->tcp_size();
	}
	static uint16_t calcTcpChecksum(_ip *ip, _tcp *tcp) {
		uint32_t ret = 0;
		tcp->_checksum = 0;
		
		ret += (ntohs(ip->src() >> 16)) + (ntohs(ip->src() & 0xFFFF));
		ret += (ntohs(ip->dst() >> 16)) + (ntohs(ip->dst() & 0xFFFF));
		ret += ip->proto();
		ret += tcp->tcp_size();
		for(int i=0; i<tcp->tcp_size(); i+=2) {
			ret += (*((uint8_t*)tcp + i) << 8) + (*((uint8_t*)tcp + i + 1));
		}
		ret += ret >> 16;
		return ntohs(~(uint16_t)ret);
	}
	enum : uint16_t {
		dns = 53,
		tls = 443
	};
	enum : uint16_t {
		flags_ack = 0x10,
		flags_psh = 0x8,
		flags_rst = 0x4,
		flags_syn = 0x2,
		flags_fin = 0x1
	};
};


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
