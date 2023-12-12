#include "tlshdr.h"

#define READ8  pkt[offset]
#define READ16 (uint16_t)(pkt[offset] << 8 | pkt[offset + 1])
#define READ24 (uint32_t)(pkt[offset + 1] << 16 | pkt[offset + 2] << 8 | pkt[offset + 3])

bool TlsHdr::parse(const uint8_t *pkt, uint32_t len) {
    uint32_t offset = 0;

    contentType_ = READ8;
    offset += 3;
    if (contentType() != TlsHdr::handshake)
        return false;

    length_ = READ16;
    offset += 2;

    type_ = READ8;
    offset += 1;
    if (type() != TlsHdr::clientHello)
        return false;

    handshakeLength_ = READ24;
    offset += 37;

    sessionIdLength_ = READ8;
    offset += sessionIdLength() + 1;

    cipherSuitesLength_ = READ16;
    offset += cipherSuitesLength() + 2;

    compMethodsLength_ = READ8;
    offset += compMethodsLength() + 1;

    extensionsLength_ = READ16;
    offset += 2;

    while (offset < len) {
        extensionType_ = READ16;
        offset += 2;

        len_ = READ16;
        offset += 2;

        if (extensionType_ == TlsHdr::typeServerName) {
            offset += 3;

            serverNameLen_ = READ16;
            offset += 2;

            serverName_ = std::string(pkt + offset, pkt + offset + serverNameLen());
            return true;
        } else {
            offset += len_;
        }
    }
    return false;
}

void TlsHdr::clear() {
    contentType_ = 0;
    length_ = 0;

    type_ = 0;
    handshakeLength_ = 0;
    sessionIdLength_ = 0;
    cipherSuitesLength_ = 0;
    compMethodsLength_ = 0;
    extensionsLength_ = 0;

    extensionType_ = 0;
    len_ = 0;

    serverNameLen_ = 0;
    serverName_.clear();
}