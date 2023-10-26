CC = g++
CFLAGS = -Wall
LDFLAGS = -lpcap
OBJS = main.o gtrace.o 
TARGET = block-packet

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: packet.h utility.h gtrace.h pdu.h main.cpp
packet.o: pdu.h packet.h packet.cpp
utility.o: pdu.h utility.h utility.cpp
gtrace.o: pdu.h gtrace.h gtrace.cpp

eth.o: hpdu.h eth.h eth.cpp
ip.o: hpdu.h ip.h ip.cpp
tcp.o: hpdu.h tcp.o tcp.cpp
udp.o: hpdu.h udp.o udp.cpp
openvpntcp.o: hpdu.h openvpntcp.o openvpntcp.cpp

clean:
	rm -f *.o
	rm -f $(TARGET)