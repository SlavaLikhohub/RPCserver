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
    
    char *result = malloc(num * (max_digits + 1));
    result[0] = '\n';

    char *buff = malloc(max_digits + 5);

    for (long i = 0; i < num; i++) {
        sprintf(buff, "%li, ", i);
        strcat(result, buff);
    }
    free(buff);
    return result;
}