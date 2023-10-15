LDLIBS += -lpcap

all: block-packet

block-packet: headers.h log.h block-packet.cpp gtrace.cpp gtrace.h

clean:
	rm -f block-packet *.o
