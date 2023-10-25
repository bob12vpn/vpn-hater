#include "hpdu.h"


struct TcpHdr {
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

	uint16_t tcp_size() { return (uint16_t)_hdr_len * 4; }
	static uint16_t len(IpHdr *ip, TcpHdr *tcp);
	static uint16_t calcTcpChecksum(IpHdr *ip, TcpHdr *tcp);
	
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