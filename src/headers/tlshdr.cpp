#include "tlshdr.h"

bool TlsHdr::parse(const uint8_t *pkt, uint32_t len) {
    uint32_t offset = 0;

    contentType_ = (uint8_t)pkt[offset];
    offset += 3;
    if (contentType() != TlsHdr::handshake)
        return false;

    length_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
    offset += 2;

    type_ = (uint8_t)pkt[offset];
    offset += 1;
    if (type() != TlsHdr::clientHello)
        return false;

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

    while (offset < len) {
        extensionType_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
        offset += 2;

        len_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
        offset += 2;

        if (extensionType_ == TlsHdr::typeServerName) {
            offset += 3;

            serverNameLen_ = (uint16_t)(pkt[offset] << 8 | pkt[offset + 1]);
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