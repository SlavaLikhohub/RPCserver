#include "rpc_logging.h"
#include <getopt.h>
#include <bsd/stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <gmodule.h>

struct args {
    bool verbose;
    unsigned int port;
    char *address;
    GList *funcs;
};

struct args parseArgs(int argc, char *argv[], bool server);