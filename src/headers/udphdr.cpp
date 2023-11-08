#include "udphdr.h"

void clear() {
    this->srcport_ = 0;
	this->dstport_ = 0;
	this->length_ = 0;
	this->checksum_ = 0;
}