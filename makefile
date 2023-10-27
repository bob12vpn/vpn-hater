TARGET=../block-packet
SRCS=src/$(wildcard *.cpp) src/headers/$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%o)

CPPFLAGS=-Wall -Wextra
LDLIBS=-lpcap

$(TARGET): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f $(TARGET) $(OBJS)