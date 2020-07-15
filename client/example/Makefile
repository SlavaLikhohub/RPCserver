DEBUG ?= 5
TARGET := RPCclient
SRCDIR = ./src
INCDIRS = ./inc

SRCS = $(TARGET).c
SRCS += parseArgs.c
SRCS += librpc.c

LIBS = glib-2.0
LIBS += libbsd
LIBS += libpcre2-8

BUILDDIR = ./.build


CFLAGS = -O2 -std=gnu17 -Wall -Wextra -Wpedantic
CFLAGS += $(shell pkg-config --cflags $(LIBS))
CFLAGS += -DDEBUG=$(DEBUG)
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -Wl,--as-needed
CC := gcc

.PHONY: all clean tidy

all: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIRS)) -c $< -o $@

$(BUILDDIR)/$(TARGET): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET): $(BUILDDIR)/$(TARGET)
	ln -sf $< $@

$(BUILDDIR):
	mkdir -p $@

clean:
	-rm -rf $(BUILDDIR)

tidy: clean
	-rm -f $(TARGET)

