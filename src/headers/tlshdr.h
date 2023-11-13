#ifndef TLSHDR_H_
#define TLSHDR_H_

#include "hpch.h"


#pragma pack(push, 1)
struct TlsHdr {
	// record field
	uint8_t contentType_;
	uint16_t length_;
	
	// handshake field
	uint8_t type_;
	uint32_t handshakeLength_;
	uint8_t sessionIdLength_;
	uint16_t cipherSuitesLength_;
	uint8_t compMethodsLength_;
	uint16_t extensionsLength_;
	
	// extension field
	uint16_t extensionType_;
	uint16_t len_;
	
	// server name field
	uint16_t serverNameLen_;
	std::string serverName_;

	uint8_t contentType() { return contentType_; }
	uint16_t length() { return length_; }
	
	uint8_t type() { return type_; };
	uint32_t handshakeLength() { return handshakeLength_; }
	uint8_t sessionIdLength() { return sessionIdLength_; }
	uint16_t cipherSuitesLength() { return cipherSuitesLength_; }
	uint8_t compMethodsLength() { return compMethodsLength_; }
	uint16_t extensionsLength() { return extensionsLength_; }
	
	uint16_t extensionType() { return extensionType_; }
	uint16_t len() { return len_; }
	
	uint16_t serverNameLen() { return serverNameLen_; }
	std::string serverName() { return serverName_; }
	
	void parse(const uint8_t*);
	
	enum : uint8_t {
        handshake = 22,
		clientHello = 1,
		typeServerName = 0
	};
};
#pragma pack(pop)

#endif // TLSHDR_H_
