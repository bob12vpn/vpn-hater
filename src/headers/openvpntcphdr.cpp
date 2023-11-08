#include "openvpntcphdr.h"

void OpenVpnTcpHdr::clear() {
    this->plen_ = 0;
	this->peerid_ = 0;
	this->type_ = 0;
}