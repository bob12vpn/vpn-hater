#include "grehdr.h"

void GreHdr::clear() {
    this->flagsAndVersion_ = 0;
    this->proto_ = 0;
    this->payloadLength_ = 0;
    this->callId_ = 0;
    this->sequenceNumber_ = 0;
	this->ackNumber_ = 0;
}

void GreHdr::setSeqAck() {
    return;
}