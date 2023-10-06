LDLIBS += -lpcap

all: drop-packet

drop-packet: headers.h log.h drop-packet.cpp gtrace.cpp gtrace.h

clean:
	rm -f drop-packet *.o
