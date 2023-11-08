#include "ppphdr.h"

void PppHdr::clear() {
    this->address_ = 0;
    this->control_ = 0;
    this->protocol_ = 0;
}