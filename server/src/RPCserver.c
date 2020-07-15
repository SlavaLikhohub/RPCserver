#define _GNU_SOURCE

#include "server.h"
#include "commondefs.h"
#include "parseArgs.h"
#include "linkFuncs.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <gmodule.h>

int main(int argc, char *argv[])
{
    struct args args = parseArgs(argc, argv, true);

    if (args.verbose)
        fprintf(stdout, "Start link functions\n");
    GList *funcs = linkFuncs(args.funcs);

    if (args.verbose)
        fprintf(stdout, "Run server\n");
    runServer(args.address, args.port, funcs, args.verbose);
}