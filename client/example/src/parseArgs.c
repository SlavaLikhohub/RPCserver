#include "parseArgs.h"
#include "rpc_logging.h"
#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *USAGE = "Usage: ./RPCserver [options] func1 func2 ...\n";
static const char *HELP = "\
OPTIONS:\n\
    --help\n\
        Show this message\n\
    --verbose\n\
        Verbouse\
    --port\n\
        Set port to listen (required)\n\
    --address\n\
        Set server's address in format '127.0.0.1'\
";

/**
 * parseArgs() - parse arguments needed for a server
 * @argc:   length of a argument list.
 * @argv:   argument list.
 * @server: if true - create a list of function name 
 *          that have to be dynamically linked.
 *
 * Parse command line arguments. 
 * Port and address parameter are required, other are optional.
 * In server @server flag should be true.
 * At least one function for dymanic linking is required.
 *
 * Return: struct args with filled fields. If @server flag is false, .func field is NULL.
 */

struct args parseArgs(int argc, char **argv, bool server)
{
    struct args argsParse = {
        .verbose = false,
        .port = 0,
        .address = NULL,
        .funcs = NULL
    };

    int _opterr = opterr;
    opterr = 0;

    const struct option long_options[] =
        {
            {"help",    no_argument,       0, 'h'},
            {"verbose", no_argument,       0, 'v'},
            {"port",    required_argument, 0, 'p'},
            {"address", required_argument, 0, 'a'},
            {0, 0, 0, 0}
        };

    const char *errstr = NULL;
    for (int sym; (sym = getopt_long(argc, argv, "vp:", long_options, &optind)) != -1;) {
        switch (sym) {
        case 'h':
            fprintf(stdout, "%s%s\n", USAGE, HELP);
            exit(EXIT_SUCCESS);
            break;
        case 'v':
            argsParse.verbose = true;
            break;
        case 'p':
            argsParse.port = strtonum(optarg, 0, 65535, &errstr);
            if (errstr) {
                fprintf(stderr, "number of iterations is %s: %s", errstr, optarg);
                exit(EINVAL);
            }
            break;
        case 'a':
            argsParse.address = malloc(strlen(optarg) + 1);
            strcpy(argsParse.address, optarg);
            break;
        default:
            fprintf(stderr, "Wrong argument\n");
            exit(EINVAL);
        }

    }
    if (argsParse.address == NULL) {
        fprintf(stderr, "No address provided\n");
        exit(EXIT_FAILURE);
    }
    if (argsParse.port == 0) {
        fprintf(stderr, "No port provided\n");
        exit(EXIT_FAILURE);
    }
    if (server) {
        log_dbg("argc: %i", argc);
        log_dbg("optind: %i", optind);
        if (argc - optind < 1) {
            fprintf(stderr, "No function to link\n");
            fprintf(stderr, USAGE);
            exit(EXIT_FAILURE);
        }

        while (optind < argc) {
            argsParse.funcs = g_list_prepend(argsParse.funcs, argv[optind++]);
        }

        opterr = _opterr;

        log_dbg("Args:\n\
            verbose: %i\n\
            port: %u\n", argsParse.verbose, argsParse.port);
        if (log_cond) {
            GList *el = argsParse.funcs;
            while (el != NULL) {
                log_dbg("%s", (char *)el->data);
                el = el->next;
            }
        }
    }
    log_dbg("Finish argsParse");
    return argsParse;
}