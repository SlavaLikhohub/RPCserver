DEBUG ?= 5
TARGET := test
SRCDIR  = .
INCDIRS = ../server/inc/

SRCS = $(TARGET).cpp

LIBS = glib-2.0
MANUAL_LINK = -lRPCserver

BUILDDIR = ./.build

CFLAGS = -O2 -Wall -Wextra -Wpedantic
# Link RPCserver
CFLAGS += -L../server/.build/test/ -Wl,-rpath=../server/.build/test/
# Link boost
CFLAGS += -I/usr/lib/

CFLAGS += $(shell pkg-config --cflags $(LIBS))
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -Wl,--as-needed
CFLAGS += $(MANUAL_LINK)
STATICLINK = /usr/lib/libboost_unit_test_framework.a
CC := g++

.PHONY: all clean tidy

all: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIRS)) -c $< -o $@

$(BUILDDIR)/$(TARGET): $(addprefix $(BUILDDIR)/,$(SRCS:.cpp=.o))
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(STATICLINK)

$(TARGET): $(BUILDDIR)/$(TARGET)
	ln -sf $< $@

$(BUILDDIR):
	mkdir -p $@

clean:
	-rm -rf $(BUILDDIR)
	-rm -rf $(FUNCBUILDDIR)

tidy: clean
	-rm -f $(TARGET)

