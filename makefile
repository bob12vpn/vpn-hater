TARGET=block-packet
SRCDIR=./src
HDRDIR=./src/headers
FILDIR=./src/filters
SRCS=$(wildcard $(HDRDIR)/*.cpp) $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(FILDIR)/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)

CPPFLAGS=-Wall -Wextra -O2 -g
LDLIBS=-lpcap

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f $(TARGET) $(OBJS)
