#define MAX 800
#include <stdbool.h>

int connectToServer(char *address, unsigned int port);
bool closeConnection(int sockfd);

char *remoteCall(char *command, int sockfd);