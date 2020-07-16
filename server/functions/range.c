#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *range(char *number)
{
    int max_digits = strlen(number);
    char *ptr;
    long num = strtol(number, &ptr, 10);
    if (num < 0)
        return NULL;
    
    char *result = (char *)malloc(num * (max_digits + 5));
    if (result == NULL) {
        char *msg = "Cannot allocate memmory";
        result = (char *)malloc(strlen(msg));
        return result;
    }
    result[0] = '\0';

    char *buff = (char *)malloc(max_digits + 5);
    if (buff == NULL) {
        free(result);
        char *msg = "Cannot allocate memmory";
        buff = (char *)malloc(strlen(msg));
        return buff;
    }

    for (long i = 0; i < num; i++) {
        sprintf(buff, "%li, ", i);
        strcat(result, buff);
    }
    free(buff);
    return result;
}