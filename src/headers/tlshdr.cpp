#include "tlshdr.h"

void TlsHdr::parse(const uint8_t *pkt) {
    int offset = 0;
    
    contentType_ = (uint8_t)pkt[offset];
    offset += 3;
    if(contentType() != TlsHdr::handshake) return;
    
    length_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
    offset += 2;
    
    type_ = (uint8_t)pkt[offset];
    offset += 1;
    if(type() != TlsHdr::clientHello) return;
    
    handshakeLength_ = (uint32_t)(pkt[offset + 1] << 16 | pkt[offset + 2] << 8 | pkt[offset + 3]);
    offset += 37;
    
    sessionIdLength_ = (uint8_t)pkt[offset];
    offset += sessionIdLength() + 1;
    
    cipherSuitesLength_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
    offset += cipherSuitesLength() + 2;
    
    compMethodsLength_ = (uint8_t)pkt[offset];
    offset += compMethodsLength() + 1;
    
    extensionsLength_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
    offset += 2;
    
    while(true) {
        extensionType_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
        offset += 2;
        
        len_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
        offset += 2;
        
        if(extensionType_ == TlsHdr::typeServerName) {
            offset += 3;
            
            serverNameLen_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
            offset += 2;
            
            serverName_ = std::string(pkt + offset, pkt + offset + serverNameLen());
            break;
        }
        else {
            offset += len_;
        }
    }
}