#include "grehdr.h"

void GreHdr::clear() {
    flagsAndVersion_ = 0;
    proto_ = 0;
    payloadLength_ = 0;
    callId_ = 0;
    sequenceNumber_ = 0;
	ackNumber_ = 0;
}

void GreHdr::setSeqAck() {
    return;
}