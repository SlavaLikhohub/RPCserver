#include "re_collect_named.h"
#include "rpc_logging.h"
#include "commondefs.h"
#include <string.h>
#include <errno.h>


// Taken from Tymofii Khodniev's simpleserver project.
long re_collect_named(const char *regexp, const char *string,
                             const char **groupnames, char **collected)
{
    if (NULL == regexp || NULL == string || NULL == groupnames || NULL == collected)
        return EINVAL;   // fail fast
    // count groups from NULL-terminated array
    long ngroups = 0;
    while (NULL != groupnames[ngroups])
        ngroups++;
    log_info("Detected %ld groups", ngroups);

    int re_err;                 // stores PCRE error code
    PCRE2_SIZE _re_erroffset;   // Not used. If we know we had an error, we don't care where
    // Use default compile context with following option flags:
    //    PCRE2_EXTENDED - Ignore white space and # comments
    //    PCRE2_UTF - Treat pattern and subjects as UTF strings
    //    PCRE2_DUPNAMES - Allow duplicate names for subpatterns
    pcre2_code *re = pcre2_compile(
        (PCRE2_SPTR)regexp,                /* A string containing expression to be compiled */
        PCRE2_ZERO_TERMINATED,             /* The length of the string or PCRE2_ZERO_TERMINATED */
        PCRE2_UTF | PCRE2_DUPNAMES,        /* Option bits */
        &re_err,                           /* Where to put an error code */
        &_re_erroffset,                    /* Where to put an error offset */
        NULL);                             /* Pointer to a compile context or NULL */
    if (NULL == re) {
        char buffer[100];
        pcre2_get_error_message(re_err, (unsigned char *)buffer, arr_len(buffer));
        log_crit("Regexp compilation error %d @ %llu : %s",
                 re_err, (long long unsigned)_re_erroffset, buffer);
        return RE_WRONG_PATTERN;
    }
    log_info("Regexp compiled");

    int ret = 0;
    // Create match block with number of entries calculated from our re
    // It will store matched results
    pcre2_match_data *match = pcre2_match_data_create_from_pattern(re, NULL);
    if (NULL == match) {
        log_crit("Match block could not be obtained");
        ret = RE_RESOURCE_ERROR;
        goto code_free;
    }


    long mcnt = pcre2_match(
        re,                                 /* Points to the compiled pattern */
        (PCRE2_SPTR)string,                 /* Points to the subject string */
        (PCRE2_SIZE)strlen(string),         /* Length of the subject string */
        (PCRE2_SIZE)0,                      /* Offset in the subject at which to start matching */
        0,                                  /* Option bits */
        match,                              /* Points to a match data block, for results */
        NULL);                              /* Points to a match context, or is NULL */

    if (mcnt < 1) {
        log_err("No matches. Match count is %ld", mcnt);
        ret = RE_NOMATCH;
        goto match_free;
    }
    log_dbg("Match count %ld", mcnt);

    if (log_cond) {
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match);
        for (long i = 0; i <  mcnt; i ++) {
            PCRE2_SPTR substring_start = string + ovector[2*i];
            PCRE2_SIZE substring_length = ovector[2*i+1] - ovector[2*i];
            log_dbg("  Item %2ld: %.*s", i, (int)substring_length, (char *)substring_start);
        }
    }

    // Now extract values of named groups
    log_info("Trying to extract %ld fields", (long)ngroups);
    long numfound = 0;
    for (long i = 0; i < ngroups; i++) {
        char *val = NULL;
        PCRE2_SIZE len = 0;
        long err = pcre2_substring_get_byname(
            match,                              /* The match data for the match */
            (PCRE2_SPTR)(groupnames[i]),      /* Name of the required substring */
            (PCRE2_UCHAR **)&val,               /* Where to put the string pointer */
            &len);                              /* Where to put the string length */
        switch (err) {
        case 0:
            break;
        case PCRE2_ERROR_UNSET:
            log_warn("PCRE group %ld (\"%s\") value not found", (long)i, groupnames[i]);
            collected[i] = NULL;
            continue;
        default:
            log_warn("PCRE group get for field %s returned %ld", groupnames[i], err);
            collected[i] = NULL;
            continue;
        }
        log_dbg("  Found %s=%s", groupnames[i], val);

        numfound++;
        // duplicate string allocating new memory as PCRE does allocation on its own
        collected[i] = strdup(val);
        if (NULL == collected[i]) {
            log_crit("Could not allocate memory for storing result %ld", (long)i);
            ret = RE_RESOURCE_ERROR;
            for (long k = i - 1; k >= 0; k--) {
                free(collected[k]);
                log_warn("Freeing %ld '%s'", (long)k, collected[k]);
            }
            goto match_free;
        }
    }

    log_info("Filled %ld matched groups", numfound);
    ret = numfound;

match_free:
    log_dbg("Deallocating re match data");
    pcre2_match_data_free(match);
code_free:
    log_dbg("Deallocating re compiled code");
    pcre2_code_free(re);
    return ret;
}