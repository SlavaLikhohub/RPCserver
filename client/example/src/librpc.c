#include "librpc.h"
#include "rpc_logging.h"

#include <arpa/inet.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <unistd.h>

/**
 * connectToServer() - connect to the appropriate RPC server
 * @address: ip address of a server in a form '127.0.0.1'
 * @port: port in which a server is listening 
 *  
 * Single attempt to connect to the RPC server via TCP connection
 * 
 * Return: if connection succed - return file descriptor to a TCP socket,
 * otherwise return -1 for failing a socket creation 
 * and -2 for failing connecting to the server
 */
int connectToServer(char *address, unsigned int port)
{
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // Socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        fprintf(stderr, "socket creation failed\n"); 
        return -1;
    } 
    log_info("Socket successfully created"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // Assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(address); 
    servaddr.sin_port = htons(port); 
    
    log_info("Connected to server: %s:%u\n", address, port);

    // Connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        fprintf(stderr, "connection with the server failed\n"); 
        return -2;
    } 
    log_info("connected to the server");

    return sockfd; 
}

/**
 * closeConnection() - close connection with RPC server
 * @sockfd: socket file descroptor for a TCP connection with RCP server.
 *          This value comes from connetToServer function
 *
 * Sending "exit" message to the server and waiting for responce, 
 * that should be "exit confirmed"
 *
 * Return: return true if exit was confirmed and false otherwise
 */
bool closeConnection(int sockfd)
{
    log_info("Send exit message\n");
    const char *exit = "exit";
    write(sockfd, exit, strlen(exit));
    char buff[MAX];
    bzero(buff, sizeof(buff)); 
    read(sockfd, buff, sizeof(buff));
    log_info("From Server : %s", buff); 
    close(sockfd);
    return strcmp("exit confirmed", buff) == 0;
}
/**
 * remoteCall() - execute funtion on RPC server.
 * @command: function name and argument to execute.
 *           Formant: func_name(argument).
 *           This library currently support only 
 *           functions with one string argument.
 * @sockfd:  socket file descroptor for a TCP connection with RCP server.
 *           This value comes from connetToServer function.
 *
 * This function send request for execute funtion on RPC server.
 * If server can find this function it executes it and send result,
 * otherwise it send back "Unknown function" message
 *
 * Return: result of function call if command is valid, "Unknown function" othewise
 */
char *remoteCall(char *command, int sockfd)
{   
    log_info("Send: %s", command);
    write(sockfd, command, strlen(command));
    char buff[MAX];
    bzero(buff, MAX);
    read(sockfd, buff, sizeof(buff));
    log_info("Reply: %s", buff);
    char *result = malloc(strlen(buff) + 1);
    return result;
}