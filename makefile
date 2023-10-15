LDLIBS += -lpcap

all: block-packet

gtrace.o: gtrace.cpp gtrace.h
	g++ -c gtrace.cpp

main.o: main.cpp headers.h gtrace.cpp gtrace.h
	g++ -c main.cpp

block-packet: main.o gtrace.o
	g++ main.o gtrace.o -lpcap -o block-packet

clean:
	rm -f block-packet *.o
