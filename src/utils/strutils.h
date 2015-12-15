/*  -------------------------------------------------------------
    RabbitMQ TCL - String utilities header library
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

     ___  __ \_____ ___  /____  /____(_)_  /___   |/  /_  __ \
     __  /_/ /  __ `/_  __ \_  __ \_  /_  __/_  /|_/ /_  / / /
     _  _, _// /_/ /_  /_/ /  /_/ /  / / /_ _  /  / / / /_/ /
     /_/ |_| \__,_/ /_.___//_.___//_/  \__/ /_/  /_/  \___\_\
                                                        _____________________
    RabbitMQ C AMQP client library TCL wrapper          ___  __/_  ____/__  /
    TCL module to connect to AMQP brokers.              __  /  _  /    __  /
                                                        _  /   / /___  _  /___
    (c) 2015, Nasqueron, some rights reserved.          /_/    \____/  /_____/
    Released under BSD-2-Clause license.

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Software:       RabbitMQ TCL
    Author:         Sébastien Santoro aka Dereckson
    Filename:       strutils.h
    Created:        2015-12-08
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*  -------------------------------------------------------------
    Magic constants
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

#define STR_NOT_FOUND -1

/*  -------------------------------------------------------------
    Common string functions inspired by PHP standard functions:

        strpos
        str_replace
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * Finds the position of the first occurrence of a substring in a string
 *
 * @param[in] haystack The string to search in
 * @param[in] needle The string to search
 * @return The position of where the needle exists, or STR_NOT_FOUND
 */
int strpos(const char *haystack, const char *needle) {
    // http://stackoverflow.com/a/7655509/1930997 - snippet by Miere
    char *p = strstr(haystack, needle);

    if (p)
        return p - haystack;

    return STR_NOT_FOUND;
}

/**
 * Replaces all occurrences of the search string with the replacement string
 *
 * @param[in] needle The value being searched for (search)
 * @param[in] replace The replacement value
 * @param[in] haystack The string being searched and replaced on (subject)
 * @return A string with the replaced values
 */
char *str_replace(const char *needle, const char *replace, char *haystack) {
    char *tok = NULL;
    char *newstr = NULL;
    char *oldstr = NULL;
    int oldstr_len = 0;
    int needle_len = 0;
    int replacement_len = 0;

    newstr = strdup(haystack);
    needle_len = strlen(needle);
    replacement_len = strlen(replace);

    if (needle == NULL || replace == NULL || needle_len == 0) {
        return newstr;
    }

    while ((tok = strstr(newstr, needle))) {
        oldstr = newstr;
        oldstr_len = strlen(oldstr);
        newstr = (char *)malloc(
            sizeof(char) * (oldstr_len - needle_len + replacement_len + 1));

        if (newstr == NULL) {
            free(oldstr);
            return NULL;
        }

        memcpy(newstr, oldstr, tok - oldstr);
        memcpy(newstr + (tok - oldstr), replace, replacement_len);
        memcpy(newstr + (tok - oldstr) + replacement_len, tok + needle_len,
               oldstr_len - needle_len - (tok - oldstr));
        memset(newstr + oldstr_len - needle_len + replacement_len, 0, 1);

        free(oldstr);
    }

    return newstr;
}
