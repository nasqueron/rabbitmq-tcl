/*  -------------------------------------------------------------
    RabbitMQ TCL - String utilities
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
    Filename:       strpos.h
    Created:        2015-12-08
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <string.h>

/*  -------------------------------------------------------------
    Magic constants
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

#define STR_NOT_FOUND -1

/*  -------------------------------------------------------------
    Common string functions: strpos
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
