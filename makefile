LDLIBS += -lpcap

all: tls-hello

tls-hello: headers.h log.h tls-hello.cpp

clean:
	rm -f tls-hello *.o
