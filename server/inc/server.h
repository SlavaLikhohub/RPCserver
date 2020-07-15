#include <gmodule.h>
#include <stdbool.h>

#define MAX 800

void runServer( const char *address, unsigned int port, 
                GList *funcs,
                bool verbose);

void workWithClient(int connfd, GList *funcs);
char *callFunc(const char *command, GList *funcs);
