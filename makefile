TARGET=block-packet
SRCDIR=./src
SRCHDRDIR=./src/headers
SRCFILDIR=./src/filters
OBJDIR=./obj
OBJHDRDIR=./obj/headers
OBJFILDIR=./obj/filters
SRCS=$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCHDRDIR)/*.cpp) $(wildcard $(SRCFILDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

CPPFLAGS=-Wall -Wextra -O2 -g
LDLIBS=-lpcap

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR) $(OBJHDRDIR) $(OBJFILDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(TARGET): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -rf $(TARGET) $(OBJDIR) block-packet.log
