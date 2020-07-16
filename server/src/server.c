#include "server.h"
#include "rpc_logging.h"
#include "commondefs.h"
#include "re_collect_named.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

int globalsockfd = -1; // To close socket after iterrupt signel was recived

/**
 * handle_sigint() - function to handle interrupt signal
 *
 * When interrupt signal (Ctrl+C) arrives this function closing socket and exits from the program.
 */
static void handle_sigint() 
{
    log_info("Got iterrupt signal");
    close(globalsockfd);
    log_info("Socket is closed");
    exit(EXIT_SUCCESS);
}

/**
 * runServer() - run a RPC server
 * @address:  IP address of a server in form "127.0.0.1".
 * @port:     port where server will listen for request
 * @funcs:    double linked list of struct func with dynamically linked functions
 * @verbose:  Verbose
 * 
 * This function runs a RPC server and waiting for connection. 
 * When request for connetion arrives server accept it 
 * and work with client with workWithClient function untill client sent "exit" message.
 * After that server close the connection and waiting for new request.
 * Server can be turned off by interrupt signal (Ctrl+C).
 */
void runServer( const char *address, 
                unsigned int port, GList *funcs,
                bool verbose)
{
    signal(SIGINT, handle_sigint); 

    int sockfd;
    struct sockaddr_in servAddr, clientAddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        fprintf(stderr, "socket creation failed");
        exit(EXIT_FAILURE);
    }
    log_info("Socket successfully created");
    bzero(&servAddr, sizeof(servAddr));

    // assign IP, PORT
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(address);
    servAddr.sin_port = htons(port);
    log_info("Server address: %s:%u", address, port);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr))) != 0) {
        fprintf(stderr, "socket bind failed");
        exit(EXIT_FAILURE);
    }
    log_info("Socket successfully binded");
    globalsockfd = sockfd;
    while (1) {
        // Now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0) {
            fprintf(stderr ,"Listen failed");
            exit(EXIT_FAILURE);
        }
        if (verbose)
            fprintf(stdout, "Listen\n");
        log_info("Server listening");

        socklen_t len = sizeof(clientAddr);

        // Accept the data packet from client and verification
        int connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &len);
        if (connfd < 0) {
            fprintf(stderr, "server acccept failed");
            exit(EXIT_FAILURE);
        }
        log_info("server acccept the client");
        if (verbose)
            fprintf(stdout, "Acccept the client\n");
        workWithClient(connfd, funcs);
        if (verbose)
            fprintf(stdout, "Done work with the client\n");
    }
}

/**
 * workWithClient() - work with client
 * @connfd: connection file desriptor.
 * @funcs:  double linked list of struct func with dynamically linked functions.
 * 
 * Waiting for client request to execute function. 
 * When request arrive call callFunc and send back the result.
 * When "exit" message arrive - return.
 */

void workWithClient(int connfd, GList *funcs)
{
    log_dbg("workWithClient\n");
    sleep(1);
    char incomeBuff[MAX];
    char outcomeBuff[MAX];
    long n;

    while (1) {
        bzero(incomeBuff, MAX);
        incomeBuff[MAX - 1] = '\0';
        bzero(outcomeBuff, MAX);
        outcomeBuff[MAX - 1] = '\0';
        n = read(connfd, incomeBuff, sizeof(incomeBuff));
        if (n < 0)
            log_err("Recieve %li", n);
        else if (n > 0)
            log_dbg("Recieve %li bytes", n);
        log_dbg("Message from the client: %s", incomeBuff);

        if(strcmp(incomeBuff, "exit") == 0) {
            log_info("Recive exit\n");
            const char *exitMsg = "exit confirmed";
            write(connfd, exitMsg, strlen(exitMsg));
            break;
        }

        char *result = callFunc(incomeBuff, funcs);
        if (result == NULL) {
            result = malloc(1);
            result[0] = '\n';
        }
        log_dbg("Function return: %s", result);
        strcpy(outcomeBuff, result);
        free(result);
        write(connfd, outcomeBuff, sizeof(outcomeBuff));
    }
}

/**
 * callFunc() - call dynamicaly linked function
 * @command: function name and argument to execute.
 *           Formant: func_name(argument).
 *           This library currently support only functions with one string argument.
 * @funcs:   double linked list of struct func with dynamically linked functions.
 *
 * Search for appropriate function if @funcs and if it exists call it.
 *
 * Return: result of function call if fucntion exist and "Unknown function" otherwise. 
 */
char *callFunc(const char *command, GList *funcs)
{
    // PARSE CALL
    static const char *regex = 
        "(?P<name>[a-zA-Z0-9_]*)\\((?P<args>.*)\\)";
    static const char *groupnames[] = {"name", "args", NULL};

    log_info("Start parsing \"%s\"", (char *)command);
    char *groups[arr_len(groupnames)];
    long found = re_collect_named(regex, command, groupnames, groups);
    log_info("Found: %li", found);
    if (found != arr_len(groupnames) - 1) {
        fprintf(stderr ,"Can not parse function call: %s\n", (char *)command);
        exit(EXIT_FAILURE);
    }
    
    char *name = groups[0];
    char *args = groups[1];
    log_dbg("name: %s", name);
    log_dbg("args: %s", args);
    
    // SEARCHING FOR A FUNCTION
    GList *el = funcs;
    while (el != NULL) {
        struct func *data = el->data;
        if (strcmp(data->name, name) == 0) {
            log_dbg("Found function");
            break;
        }
        el = el->next;
    }
    if (el == NULL) {
        fprintf(stderr, "Unknown function call: %s\n", name);
        char *msg = "Unknown function";
        char *result = malloc(strlen(msg));
        strcpy(result, msg);
        return result;
    }
    // FUNCTION CALL
    struct func *data = el->data;
    log_dbg("Try to call function");
    char *result = data->funcPtr(args);

    log_dbg("Function returned: %s", result);
    if (result == NULL) {
        result = malloc(1);
        result[0] = '\0';
    }
    return result;
}
