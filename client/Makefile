DEBUG ?= 5
TARGET := librpc.so
SRCDIR = ./src
INCDIRS = ./inc

SRCS = librpc.c

LIBS = glib-2.0
LIBS += libbsd
LIBS += libpcre2-8

BUILDDIR = ./.build


CFLAGS = -O2 -std=gnu17 -Wall -Wextra -Wpedantic
CFLAGS += $(shell pkg-config --cflags $(LIBS))
CFLAGS += -DDEBUG=$(DEBUG)
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -Wl,--as-needed
CC := gcc

.PHONY: all clean tidy run install

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) -c $(CFLAGS) $(addprefix -I,$(INCDIRS)) -fpic $< -o $@

$(BUILDDIR)/$(TARGET): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILDDIR):
	mkdir -p $@

clean:
	-rm -rf $(BUILDDIR)

tidy: clean
	-rm -f $(TARGET)
