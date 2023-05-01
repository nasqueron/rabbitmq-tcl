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
    // This code is based on a str_replace function designed by
    // Chantra <chantra@debuntu.org> and Iain R. Learmonth <irl@sdf.org>
    // Source: https://github.com/irl/la-cucina/blob/master/str_replace.c

    char *token = NULL;
    char *replacedString = NULL;
    char *oldString = NULL;

    int oldStringLen = 0;
    int needleLen = 0;
    int replaceLen = 0;

    int len; // Stores len before a malloc call

    replacedString = strdup(haystack);
    needleLen = strlen(needle);
    replaceLen = strlen(replace);

    if (needle == NULL || replace == NULL || needleLen == 0) {
        return replacedString;
    }

    while ((token = strstr(replacedString, needle))) {
        oldString = replacedString;
        oldStringLen = strlen(oldString);

        len = oldStringLen - needleLen + replaceLen + 1;
        replacedString = (char *)malloc(sizeof(char) * len);

        if (replacedString == NULL) {
            free(oldString);
            return NULL;
        }

        memcpy(replacedString, oldString, token - oldString);
        memcpy(replacedString + (token - oldString), replace, replaceLen);
        memcpy(replacedString + (token - oldString) + replaceLen,
               token + needleLen,
               oldStringLen - needleLen - (token - oldString));
        memset(replacedString + oldStringLen - needleLen + replaceLen, 0, 1);

        free(oldString);
    }

    return replacedString;
}
