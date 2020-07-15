#include <string.h>
#include <stdlib.h>

char *hello(void) {
    char * result = malloc(strlen("Hello from the server") + 1);
    strcpy(result, "Hello from the server");
    return result;
}
