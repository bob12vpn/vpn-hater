#include "headers/eth.h"
#include "headers/ip.h"
#include "headers/tcp.h"
#include "headers/openvpntcp.h"

struct RxPacket {
    struct EthHdr *eth{nullptr};
    
    struct IpHdr *ip{nullptr};
    
    struct TcpHdr *tcp{nullptr};
    struct UdpHdr *udp{nullptr};
}

struct RxOpenVpnTcpPacket : RxPacket {
    struct OpenVpnTcpHdr *openvpntcp{nullptr};
}

#pragma pack(push, 1)
struct TxPacket {
    struct EthHdr eth;
    struct IpHdr ip;
    struct TcpHdr tcp;
}
#pragma pack(pop)