#define PCRE2_CODE_UNIT_WIDTH CHAR_BIT
#include <pcre2.h>  

enum re_err {
    RE_OK = 0,
    RE_WRONG_ARGS = -1,
    RE_WRONG_PATTERN = -2,
    RE_RESOURCE_ERROR = -3,
    RE_NOMATCH = -4
};

long re_collect_named(const char *regexp, const char *string,
                             const char **groupnames, char **collected);