#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifdef DEBUG
#define LOG(MSG) printf("[LOG] %s/%s(%d): %s\n", __FILE__, __FUNCTION__, __LINE__, MSG);
#define PRINT(x) printf("[VAR] %s/%s(%d): %s = %x\n", __FILE__, __FUNCTION__, __LINE__, #x, x);
#else
#define LOG(MSG) ((void)0);
#define PRINT(x) ((void)0);
#endif	// DEBUG

#endif	// LOG_H_
