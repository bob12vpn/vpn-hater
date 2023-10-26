#include "pdu.h"

pcap_t* open_pcap(char* interface);
int open_raw_socket(char* interface);
void send_packet(int socket, struct sockaddr_in addr_in_, TxPacket *pkt);
uint8_t hex2int(char n);
uint8_t* resolve_mac(char* interface);
bool custom_filter(RxOpenVpnTcpPacket *pkt);
