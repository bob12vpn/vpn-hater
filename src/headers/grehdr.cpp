#include "grehdr.h"

void GreHdr::setSeqAck() {
    if(!(flagsAndVersion_ & GreHdr::seq)) {
        if(flagsAndVersion_ & GreHdr::ack) {
            ackNumber_ = sequenceNumber_;
            sequenceNumber_ = 0;
        }
    }
}