#define _GNU_SOURCE
#include "librpc.h"
#include "parseArgs.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct args args = parseArgs(argc, argv, false);

    int sockfd = connectToServer(args.address, args.port);
    if (sockfd < 0) {
        fprintf(stderr, "Cannot connect to the server\n");
        exit(EXIT_FAILURE);
    }
    remoteCall("hello()", sockfd);
    remoteCall("helloName(Forest)", sockfd);
    remoteCall("range(100)", sockfd);
    closeConnection(sockfd);

    exit(EXIT_SUCCESS);
}
