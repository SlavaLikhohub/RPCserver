#include "linkFuncs.h"
#include "rpc_logging.h"
#include "commondefs.h"
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Directory for store functions .so files
static const char *funcDir = "./functions/.build";

/**
 * linkFuncs() - Link functions
 * @funcNames: double linked list of functions names.
 *
 * Dynamicaly links function that was specified as command line arguments.
 * If function cannot find or read function that should be linked, program exits.
 *
 * Return: double linked list of `struct func`.
 */
GList *linkFuncs(GList *funcNames)
{
    GList *funcs = NULL;
    GList *funcName = funcNames;


    while (funcName != NULL) {
        log_dbg("Start process function %s", funcName->data);
        struct func *funcEl = malloc(sizeof(struct func));
        
        log_dbg("Copy name to a struct");
        funcEl->name = malloc(strlen(funcName->data));
        strcpy(funcEl->name, funcName->data);

        // LOADING FUNCTION AS DYNAMIC LIBRARY
        log_dbg("Create file name");
        char *ldname = malloc(strlen((char *)funcEl->name) + strlen(funcDir) + 10);
        sprintf(ldname, "%s/%s.so", funcDir, (char *)funcEl->name);

        log_info("Trying to load func '%s' from '%s'", (char *)funcEl->name, ldname);
        void *handle = dlopen(ldname, RTLD_NOW | RTLD_LOCAL);
        if (handle == NULL) {
            fprintf(stderr, "Cannot open a dynamic library: %s\n", ldname);
            exit(EXIT_FAILURE);
        }
        void *dlfunc = dlsym(handle, funcEl->name);
        if (dlfunc == NULL) {
            fprintf(stderr, "Cannot read a function: %s\n", (char *)funcEl->name);
            exit(EXIT_FAILURE);
        }
        funcEl->funcPtr = dlfunc;

        // PREPENDING ELEMENT TO THE LIST
        log_info("Prepending element to the list\n");
        funcs = g_list_prepend(funcs, funcEl);

        funcName = funcName -> next;
    }
    return funcs;
}

