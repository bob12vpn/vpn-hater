#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifdef DEBUG
#define LOG(MSG) printf("[LOG] %s/%s(%d): %s\n", __FILE__, __FUNCTION__, __LINE__, MSG);
#define PRINT(x) printf("[VAR] %s/%s(%d): %s = %x\n", __FILE__, __FUNCTION__, __LINE__, #x, x);
#define PRINT_TCP(p) printf("[TCP] %s/%s(%d): %s \n %02x:%02x:%02x:%02x:%02x:%02x | %02x:%02x:%02x:%02x:%02x:%02x \n %d.%d.%d.%d | %d.%d.%d.%d \n %d -> %d \n %u vs %u \n %d \n -------- \n",\
		__FILE__, __FUNCTION__, __LINE__, #p,\
		p->eth.src[0], p->eth.src[1], p->eth.src[2], p->eth.src[3], p->eth.src[4], p->eth.src[5],\
		p->eth.dst[0], p->eth.dst[1], p->eth.dst[2], p->eth.dst[3], p->eth.dst[4], p->eth.dst[5],\
		p->ip.srcmask[0], p->ip.srcmask[1], p->ip.srcmask[2], p->ip.srcmask[3],\
		p->ip.srcmask[0], p->ip.srcmask[1], p->ip.dstmask[2], p->ip.dstmask[3],\
		ntohs(p->tcp.srcport), ntohs(p->tcp.dstport),\
		ntohl(p->tcp.seq_raw), ntohl(p->tcp.ack_raw),\
		ntohs(p->tcp.checksum)\
		);
#else
#define LOG(MSG) ((void)0);
#define PRINT(x) ((void)0);
#define PRINT_TCP(p) ((void(0));
#endif	// DEBUG

#endif	// LOG_H_
