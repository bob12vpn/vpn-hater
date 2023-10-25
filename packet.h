#include "headers/eth.h"
#include "headers/ip.h"
#include "headers/tcp.h"
#include "headers/openvpntcp.h"

#pragma pack(push, 1)
struct struct RxOpenVpnTcpPacket final {
    struct EthHdr *eth;
    struct IpHdr *ip;
    struct TcpHdr *tcp;
    struct OpenVpnTcpHdr *openvpntcp;
}
#pragma pack(pop)

#pragma pack(push, 1)
struct struct TxTcpPacket final {
    struct EthHdr eth;
    struct IpHdr ip;
    struct TcpHdr tcp;
}
#pragma pack(pop)