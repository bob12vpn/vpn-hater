#include "filtermanager.h"

FilterManager::FilterManager() {
    push_back(&l2tpFilter);
    push_back(&openVpnTcpFilter);
    push_back(&pptpFilter);
    push_back(&protonFilter);
    push_back(&sniFilter);
    // push_back(&tcpAckFilter);
}

bool FilterManager::openRawSockets(char *sendInterface) {
    for (Filter *filter : *this) {
        if (!filter->openRawSocket(sendInterface)) {
            return false;
        }
    }
    return true;
}

bool FilterManager::processAll(RxPacket *rxPacket) {
    for (Filter *filter : *this) {
        if (filter->process(rxPacket)) {
            return true;
        }
    }
    return false;
}
