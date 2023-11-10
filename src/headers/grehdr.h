#ifndef GREHDR_H_
#define GREHDR_H_

#include "hpch.h"


#pragma pack(push, 1)
struct GreHdr {
	uint16_t flagsAndVersion_;
	uint16_t proto_;
	uint16_t payloadLength_;
    uint16_t callId_;
    uint32_t sequenceNumber_;
	uint32_t ackNumber_;

	uint16_t flagsAndVersion() { return ntohs(flagsAndVersion_); }
    uint16_t proto() { return ntohs(proto_); }
    uint16_t payload_length() { return ntohs(payloadLength_); }
    uint16_t callId() { return ntohs(callId_); }
    uint32_t sequenceNumber() { return ntohl(sequenceNumber_); }
	uint32_t ackNumber() { return ntohl(ackNumber_); }
    
	uint8_t greHdrSize();
	void setSeqAck();
	
	enum : uint16_t {
		ack = 0x80,
		seq = 0x1000
	};
	enum : uint16_t {
		ppp = 0x880b
	};
};
#pragma pack(pop)

#endif // GREHDR_H_
