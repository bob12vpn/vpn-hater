#include "pch.h"

#include "gtrace.h"
#include "packet.h"
#include "rawsock.h"

#include "filters/filter.h"
#include "filters/filtermanager.h"

void usage() {
    printf("usage: sudo ./block-packet <mirror interface> <send interface> [sni list]\n");
    printf("example: sudo ./block-packet enp0s3 eth0\n");
    printf("example: sudo ./block-packet enp0s3 eth0 sni.txt\n");
}

int main(int argc, char *argv[]) {
    gtrace_default("127.0.0.1", 8908, 1, "block-packet.log");
    GTRACE("Logging level : Message");

    if (argc < 3) {
        usage();
        return -1;
    }
    char *mirrorInterface = argv[1];
    char *sendInterface = argv[2];

    RxPacket *rxPacket = new RxPacket;
    if (!rxPacket->openPcap(mirrorInterface))
        return -1;

    FilterManager filterManager;
    if (!filterManager.openRawSockets(sendInterface))
        return -1;

    if (argc == 4) {
        if (!filterManager.sniFilter.loadSni(argv[3]))
            return -1;
    }

    int pktCnt = 0;
    while (true) {
        if (!rxPacket->capture()) {
            usleep(1);
            continue;
        }

        pktCnt++;

        rxPacket->clear();
        rxPacket->parse();

        if (filterManager.processAll(rxPacket))
            GTRACE("========%d========", pktCnt);
    }

    return 0;
}
