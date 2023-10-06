#define SHOW_THREAD_ID

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#ifdef WIN32
#include <winsock2.h>
#endif // WIN32
#ifdef __linux__
#include <arpa/inet.h>
#endif // __linux__

#ifdef SHOW_THREAD_ID
#include <pthread.h>
#endif // SHOW_THREAD_ID

#include "gtrace.h"

#define BUFSIZE 8192

struct {
	struct {
#ifdef __cplusplus
		bool configured{false};
		bool active{false};
#else
		bool configured;
		bool active;
#endif // __cplusplus
	} status;

	struct {
		bool enabled;
		int sock;
		struct sockaddr_in addr_in;
	} udp;

	//
	// stderr
	//
	struct {
		bool enabled;
	} se;

	//
	// file
	//
	struct {
		bool enabled;
		FILE* fp;
	} file;

	//
	// def(ault)
	//
	struct {
#ifdef __cplusplus
		char ip[BUFSIZE]{"127.0.0.1"};
		int port{8908};
		bool se{true};
		char file[BUFSIZE]{""};
#else
		char ip[BUFSIZE];
		int port;
		bool se;
		char file[BUFSIZE];
#endif // __cplusplus
	} def;
#ifdef __cplusplus
} _gtrace;
#else
} _gtrace = {
	.status.configured = false,
	.status.active = false,
	.def.ip[0] = '1',
	.def.ip[1] = '2',
	.def.ip[2] = '7',
	.def.ip[3] = '.',
	.def.ip[4] = '0',
	.def.ip[5] = '.',
	.def.ip[6] = '0',
	.def.ip[7] = '.',
	.def.ip[8] = '1',
	.def.ip[9] = '\0',
	.def.port = 8908,
	.def.se = true,
	.def.file[0] = '\0'
};
#endif

// ----------------------------------------------------------------------------
// api
// ----------------------------------------------------------------------------
void gtrace(const char* fmt, ...) {
	if (!_gtrace.status.configured) {
		_gtrace.status.configured = true;
		bool file_load = false;
		FILE* fp = fopen("gtrace.conf", "r");
		if (fp != NULL) {
			char ip[BUFSIZE];
			int port;
			int se;
			char file[BUFSIZE];
			int res = fscanf(fp, "%s %d %d %s", ip, &port, &se, file);
			if (res >=2 && res <= 4) {
				switch (res) {
					case 2: gtrace_open(ip, port, false, NULL); break;
					case 3: gtrace_open(ip, port, (bool)se, NULL); break;
					case 4: gtrace_open(ip, port, (bool)se, file); break;
				}
				file_load = true;
				fclose(fp);
			}
		}
		if (!file_load)
			gtrace_open(_gtrace.def.ip, _gtrace.def.port, _gtrace.def.se, _gtrace.def.file);
	}

	if (!_gtrace.status.active)
		return;

	int res;
	char buf[BUFSIZE + 1];
	char* p = buf;
	int len = 0;
	ssize_t remn = BUFSIZE;

#ifdef WIN32
	SYSTEMTIME now;
	::GetLocalTime(&now);
	res = snprintf(p, remn, "%02d%02d%02d %02d%02d%02d-%03d ",
		now.wYear % 100, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);
#endif // WIN32
#ifdef __linux__
	struct timeval now;
	struct tm* local;
	gettimeofday(&now, NULL);
	local = localtime(&now.tv_sec);
	res = snprintf(p, remn, "%02d%02d%02d %02d:%02d:%02d.%03lu ",
		(local->tm_year) % 100, local->tm_mon + 1, local->tm_mday,
		local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);
#endif // __linux__
	if (res < 0) {
		fprintf(stderr, "time: snprintf return %d\n", res);
		return;
	}

	p += res; len += res; remn -= res;
	if (remn <= 0) {
		fprintf(stderr, "time: not enough buffer size res=%d len=%d\n", res, len);
		return;
	}

#ifdef SHOW_THREAD_ID
	unsigned id = (unsigned)(pthread_self() & 0xFFFF);
	res = snprintf(p, remn, "%04X ", id);
	if (res < 0) {
		fprintf(stderr, "thread: snprintf return %d\n", res);
		return;
	}
	p += res; len += res; remn -= res;
	if (remn <= 0) {
		fprintf(stderr, "thread: not enough buffer size res=%d len=%d\n", res, len);
		return;
	}
#endif // SHOW_THREAD_ID

	va_list args;
	va_start(args, fmt);
	res = vsnprintf(p, remn, fmt, args);
	va_end(args);
	if (res < 0) {
		fprintf(stderr, "vsnprintf: snprintf return %d\n", res);
		return;
	}
	p += res; len += res; remn -= res;
	if (remn <= 0) {
		fprintf(stderr, "vsprintf: not enough buffer size res=%d len=%d\n", res, len);
		return;
	}

	memcpy(p, "\n\0", 2);
	res = 2;
	/*p += res;*/ len += res; remn -= res;
	if (remn <= 0) {
		fprintf(stderr, "linefeed: not enough buffer size res=%d len=%d\n", res, len);
		return;
	}

	if (_gtrace.udp.enabled)
		sendto(_gtrace.udp.sock, buf, len - 1, 0, (struct sockaddr*)&_gtrace.udp.addr_in, sizeof(struct sockaddr_in));

	if (_gtrace.se.enabled) {
		fprintf(stderr, "%s", buf);
	}

	if (_gtrace.file.enabled) {
		fprintf(_gtrace.file.fp, "%s", buf);
		fflush(_gtrace.file.fp);
	}
}

void gtrace_default(const char* ip, int port, bool se /*stderr*/, const char* file) {
	if (ip != NULL && strlen(ip) > 0 && port != 0) {
		strncpy(_gtrace.def.ip, ip, BUFSIZE - 1);
		_gtrace.def.port = port;
	} else {
		memset(_gtrace.def.ip, 0, BUFSIZE);
		_gtrace.def.port = 0;
	}
	_gtrace.def.se = se;
	if (file != NULL && strlen(file) > 0)
		strncpy(_gtrace.def.file, file, BUFSIZE - 1);
	else
		memset(_gtrace.def.file, 0, BUFSIZE);
}

bool gtrace_open(const char* ip, int port, bool se, const char* file) {
	_gtrace.status.configured = true;
	if (_gtrace.status.active)
		return false;

#ifdef WIN32
	static bool first = true;
	if (first) {
		WSADATA wsaData;
		WSAStartup(0x0202, &wsaData);
		first = false;
	}
#endif // WIN32

	//
	// udp
	//
	_gtrace.udp.enabled = false;
	if (ip != NULL && strlen(ip) > 0 && port != 0) {
		_gtrace.udp.sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (_gtrace.udp.sock == -1) {
			fprintf(stderr, "socket return null\n");
		} else {
			_gtrace.udp.addr_in.sin_family = AF_INET;
			_gtrace.udp.addr_in.sin_port = htons(port);
			_gtrace.udp.addr_in.sin_addr.s_addr = inet_addr(ip);
			memset(&_gtrace.udp.addr_in.sin_zero, 0, sizeof(_gtrace.udp.addr_in.sin_zero));
			_gtrace.udp.enabled = true;
		}
	}

	//
	// stderr
	//
	_gtrace.se.enabled = false;
	if (se == true) {
		_gtrace.se.enabled = true;
	}

	//
	// file
	//
	_gtrace.file.enabled = false;
	if (file != NULL && strlen(file) > 0) {
		_gtrace.file.fp = fopen(file, "a");
		if (_gtrace.file.fp == NULL) {
			fprintf(stderr, "fopen(%s) return null\n", file);
		} else {
			_gtrace.file.enabled = true;
		}
	}

	_gtrace.status.active = true;
	return true;
}

bool gtrace_close(void) {
	_gtrace.status.configured = true;
	if (!_gtrace.status.active)
		return false;

	if (_gtrace.udp.enabled) {
		if (_gtrace.udp.sock != -1) {
			close(_gtrace.udp.sock);
			shutdown(_gtrace.udp.sock, 0x02); /* SD_BOTH */
			_gtrace.udp.sock = -1;
		}
	}

	if (_gtrace.se.enabled) {
	}

	if (_gtrace.file.enabled) {
		if (_gtrace.file.fp != NULL) {
			fclose(_gtrace.file.fp);
			_gtrace.file.fp = NULL;
		}
	}

	_gtrace.status.active = false;
	return true;
}

// ----------------------------------------------------------------------------
// macro
// ----------------------------------------------------------------------------
const char* gtrace_file_name(const char* file_name) {
#ifdef WIN32
	const char* p1 = strrchr(file_name, '\\');
	const char* p2 = strrchr(file_name, '/');
	const char* p	= p1 > p2 ? p1 : p2;
#endif // WIN32
#ifdef __linux__
	const char* p = strrchr(file_name, '/');
#endif // __linux__
	return (p == NULL ? file_name : p + 1);
}

const char* gtrace_func_name(const char* func_name) {
#ifdef WIN32
	const char* p = strrchr(func_name, ':');
	return (p == NULL ? func_name : p + 1);
#endif // WIN32
#ifdef __linux__
	return func_name;
#endif // __linux__
}
