DEBUG ?= 5
TARGET := RPCserver
SRCDIR  = ./src
INCDIRS = ./inc

SRCS = $(TARGET).c
SRCS += parseArgs.c
SRCS += linkFuncs.c
SRCS += server.c
SRCS += re_collect_named.c

FUNC  = helloName.c
FUNC += hello.c

LIBS = glib-2.0
LIBS += libbsd
LIBS += libpcre2-8

MANUAL_LINK = -ldl

FUNCDIR = ./functions
FUNCBUILDDIR = $(FUNCDIR)/.build

BUILDDIR = ./.build

CFLAGS = -O2 -std=gnu17 -Wall -Wextra -Wpedantic
CFLAGS += $(shell pkg-config --cflags $(LIBS))
CFLAGS += -DDEBUG=$(DEBUG)
CFLAGSSL = -O2 -std=gnu17 -Wall -Wextra -Wpedantic
SHAREDFLAG = -lm -shared
LDFLAGS = $(MANUAL_LINK)
LDFLAGS += $(shell pkg-config --libs $(LIBS)) -Wl,--as-needed
CC := gcc

.PHONY: all clean tidy run functions

all: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIRS)) -c $< -o $@

$(BUILDDIR)/$(TARGET): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET): $(BUILDDIR)/$(TARGET)
	ln -sf $< $@

$(BUILDDIR):
	mkdir -p $@

functions:$(FUNCBUILDDIR)/*.so

$(FUNCBUILDDIR)/%.so:$(FUNCDIR)/%.c | $(FUNCBUILDDIR)
	$(foreach file, $(wildcard $(FUNCDIR)/*.c),\
	$(CC) $(CFLAGSSL) $(file) $(SHAREDFLAG) -o $(file:.c=.so);\
	mv $(file:.c=.so) $(FUNCBUILDDIR)/;)
	
$(FUNCBUILDDIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET) -v --address '127.0.0.1' --port 10080 hello helloName range

clean:
	-rm -rf $(BUILDDIR)
	-rm -rf $(FUNCBUILDDIR)

tidy: clean
	-rm -f $(TARGET)
