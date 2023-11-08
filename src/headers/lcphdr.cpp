#include "lcphdr.h"

void LcpHdr::clear() {
    this->code_ = 0;
    this->identifier_ = 0;
    this->length_ = 0;
}