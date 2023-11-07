#include "ethhdr.h"

void EthHdr::clear() {
    for(int i=0; i<6; i++) {
        dst_[i] = 0;
        src_[i] = 0;
    }
    type_ = 0;
}