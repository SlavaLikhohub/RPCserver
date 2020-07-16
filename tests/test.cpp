#define BOOST_TEST_MODULE CallFuncTest
#include <boost/test/auto_unit_test.hpp>
#include <string>
#include <iostream>
extern "C" 
{
#include "../server/inc/commondefs.h"
#include "../server/inc/linkFuncs.h"
#include "../server/inc/parseArgs.h"
#include "../server/inc/re_collect_named.h"
#include "../server/inc/rpc_logging.h"
#include "../server/inc/server.h"
#include "../server/functions/range.c"
#include <gmodule.h>
}
/*
 * Test callFunc function from server.c
 * To build test call **make testbuild** command in **server** directory 
 * and **make** in **test** directory
 */
BOOST_AUTO_TEST_CASE(CallFuncTest)
{
    GList *funcs = NULL;
    struct func *funcEl = static_cast<typeof(funcEl)>(malloc(sizeof(struct func)));
    
    const char *name = "range";
    funcEl->name = static_cast<typeof(funcEl->name)>(malloc(strlen(name) + 1));
    strcpy(funcEl->name, name);
    funcEl->funcPtr = range;

    funcs = g_list_prepend(funcs, funcEl);
    

    for (int i = -300; i <= 5000; i += 100) {
        std::string command = "range(";
        command += std::to_string(i);
        command += ")";
        BOOST_TEST_CHECKPOINT(command);

        
        char *argument = static_cast<typeof(argument)>(malloc(std::to_string(i).size() + 1));
        strcpy(argument, std::to_string(i).c_str());

        char * result = callFunc(command.c_str(), funcs);
        char * resultDirect = range(argument);
        if (resultDirect == NULL) {
            resultDirect = static_cast<char *>(malloc(1));
            resultDirect[0] = '\0';
        }
        BOOST_CHECK(strcmp(result, resultDirect) == 0);
        free(result);
        free(resultDirect);
        free(argument);
    }
}