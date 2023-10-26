#ifndef UDP_H_
#define UDP_H_

#include "hpdu.h"


#define	UDP_SIZE	8


struct UdpHdr {
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

#endif // UDP_H_
