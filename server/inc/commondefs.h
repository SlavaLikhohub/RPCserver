#ifndef _COMMONDEFS_H
#define _COMMONDEFS_H

#define arr_len(array) (sizeof (array) / sizeof (*(array)))

/**
 * struct func - Structure to save function name and pointer.
 */
struct func {
    /** Name of the function. */
    char *name;
    /** Pointer to the function. */
    char *(*funcPtr)(char *);
};


#endif