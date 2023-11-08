#include "iphdr.h"

uint16_t IpHdr::calcIpChecksum(IpHdr *iphdr) {
    uint32_t ret = 0;
    uint16_t *pword = reinterpret_cast<uint16_t*>(iphdr);
    iphdr->checksum_ = 0;
    
    for(int i=0; i<iphdr->ipHdrSize(); i+=2) {
        ret += ntohs(*(pword + i));
    }
    ret += ret >> 16;
    return ntohs(~(uint16_t)ret);
}

void IpHdr::clear() {
    this->hdrLen_ = 0;
	this->version_ = 0;
	this->dsfield_ = 0;
	this->len_ = 0;
	this->id_ = 0;
	this->flags_ = 0;
	this->fragOffset_ = 0;
	this->ttl_ = 0;
	this->proto_ = 0;
	this->checksum_ = 0;
	this->src_ = 0;
	this->dst_ = 0;
}
