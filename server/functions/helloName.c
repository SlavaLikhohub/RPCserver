#include <string.h>
#include <stdlib.h>

char *helloName(char *name) {
    char *result = malloc(strlen("Hello, ") + strlen(name) + 1);
    strcpy(result, "Hello, ");
    strcat(result, name);    
    return result;    
}
