LDLIBS += -lpcap

all: drop-packet

drop-packet: headers.h log.h drop-packet.cpp

clean:
	rm -f drop-packet *.o
