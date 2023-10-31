#include "rawsock.h"

bool RawSock::open(char* interface) {
    if(socket) return false;
    
    socket = ::socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
    if(!socket) {
        GTRACE("socket return -1");
        return false;
    }
    
    int one = 1;
    int *pOne = &one;
    if(::setsockopt(socket, IPPROTO_IP, IP_HDRINCL, pOne, sizeof(one))) {
        GTRACE("setsockopt(IP_HDRINCL) Failed");
        return false;
    }
    
    if(strlen(interface) != 0) {
        if(::setsockopt(socket, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface))) {
            GTRACE("setsockopt(SO_BINDTODEVICE) Failed");
            return false;
        }
    }
    
    struct sockaddr_in addr_in_;
    memset(&addr_in_, 0, sizeof(addr_in_));
    addr_in_.sin_family = AF_INET;
    
    GTRACE("socket is opened");
    return true;
}

bool RawSock::close() {
    if(!socket)
        return false;
    ::close(socket);
    socket = 0;
    GTRACE("close socket");
    return true;
}

bool RawSock::sendto(TxPacket *pkt, char *msg) {
    addr_in_.sin_addr.s_addr = pkt->ip.dst();
    if(::sendto(socket, &(pkt->ip), pkt->ip.len(), 0, (struct sockaddr*)&addr_in_, sizeof(struct sockaddr_in)) == -1) {
        GTRACE("[Error] send packet is failed");
        return false;
    }
    GTRACE("send %s packet", msg);
    return true;
}
