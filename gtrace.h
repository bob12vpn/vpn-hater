// ----------------------------------------------------------------------------
//
// gtrace version 1.9
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __GTRACE_H__
#define __GTRACE_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------
// api
// ----------------------------------------------------------------------------
void gtrace(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void gtrace_default(const char* ip, int port, bool se /*stderr*/, const char* file);
bool gtrace_open(const char* ip, int port, bool se /*stderr*/, const char* file);
bool gtrace_close();

// ----------------------------------------------------------------------------
// macro
// ----------------------------------------------------------------------------
const char* gtrace_file_name(const char* file_name);
const char* gtrace_func_name(const char* func_name);

#define GTRACE(fmt, ...) \
do { \
	gtrace("[%s:%d] %s " fmt, gtrace_file_name(__FILE__), __LINE__, gtrace_func_name(__FUNCTION__), ##__VA_ARGS__); \
} while(0)

#ifdef __cplusplus
}
#endif

#endif // __GTRACE_H__
