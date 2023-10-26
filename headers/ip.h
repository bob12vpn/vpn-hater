#include "hpdu.h"


struct IpHdr {
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

	uint8_t version() { return _hdr_len; }
	uint8_t hdr_len() { return _version; }
	uint16_t len() { return ntohs(_len); }
	uint8_t proto() { return _proto; }
	uint16_t checksum() { return ntohs(_checksum); }
	uint32_t src() { return _src; }	// little endian, for simple implementation
	uint32_t dst() { return _dst; }	// little endian, same reason

	uint16_t ip_size() { return (uint16_t)hdr_len() * 4; }
	static uint16_t calcIpChecksum(IpHdr *ip);
    
	enum : uint16_t {
		tcp = 6,
		udp = 17
	};
};