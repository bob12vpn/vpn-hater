#include "ethhdr.h"

void EthHdr::clear() {
    for(int i=0; i<6; i++) {
        this->dst_[i] = 0;
        this->src_[i] = 0;
    }
    this->type_ = 0;
}