#include "grehdr.h"

uint8_t GreHdr::greHdrSize() {
    if(flagsAndVersion() & GreHdr::seq) {
        if(flagsAndVersion() & GreHdr::ack) {
            return 16;
        }
    }
    return 12;
}

void GreHdr::setSeqAck() {
    if(!(flagsAndVersion() & GreHdr::seq)) {
        if(flagsAndVersion() & GreHdr::ack) {
            ackNumber_ = sequenceNumber_;
            sequenceNumber_ = 0;
        }
    }
}