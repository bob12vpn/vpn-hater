TARGET=block-packet
SRCDIR=./src
HDRDIR=./src/headers
SRCS=$(wildcard $(HDRDIR)/*.cpp) $(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)

CPPFLAGS=-Wall -Wextra
LDLIBS=-lpcap

$(TARGET): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f $(TARGET) $(OBJS)
