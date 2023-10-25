#include "hpdu.h"


struct IpHdr {
	uint8_t _hdr_len;
	uint8_t _dsfield;
	uint16_t _len;
	uint16_t _id;
	uint16_t _off;
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
    uint32_t *opt;

	uint8_t hdr_len() { return _hdr_len & 0x0F; }
	uint8_t version() { return _hdr_len & 0xF0; }
	uint16_t len() { return ntohs(_len); }
	uint8_t proto() { return _proto; }
	uint16_t checksum() { return ntohs(_checksum); }
	uint32_t src() { return _src; }	// big endian, for simple implementation
	uint32_t dst() { return _dst; }	// big endia, same reason
	uint8_t* srcmask() {
		uint8_t *ret = new uint8_t[4];
		ret = reinterpret_cast<uint8_t*>(&_src);
		return ret;
	}
	uint8_t* _dstmask() {
		uint8_t *ret = new uint8_t[4];
		ret = reinterpret_cast<uint8_t*>(&_dst);
		return ret;
	}

	uint16_t hdr_size() { return (uint16_t)_hdr_len * 4; }
	static uint16_t calcIpChecksum(IpHdr *ip);
    
	enum : uint16_t {
		tcp = 6,
		udp = 17
	};
};