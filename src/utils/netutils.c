/*  -------------------------------------------------------------
    RabbitMQ TCL - Net helper functions
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
    Filename:       netutils.c
    Created:        2015-12-08
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <stdlib.h>
#include <string.h>
#include "strutils.h"

/*  -------------------------------------------------------------
    Parser for host:port expressions
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * Gets host from a host:port expression
 *
 * @param[in] expression the host:port expression
 * @return the hostname
 */
char *get_host(const char *expression, const char *defaultHost) {
    char *host = malloc(1024 * sizeof(char));
    int pos;

    // If the expression is empty, the default host is privileged
    if (expression[0] == '\0') {
        strcpy(host, defaultHost);
        return host;
    }

    // Gets the host part of an host:port expression
    pos = strpos(expression, ":");
    if (pos == STR_NOT_FOUND) {
        strcpy(host, expression);
    } else if (pos == 0) {
        strcpy(host, defaultHost); // Handle ":5000" case
    } else {
        strncpy(host, expression, pos);
        host[pos] = '\0';
    }

    return host;
}

/**
 * Gets port from a host:port expression
 *
 * @param[in] expression the host:port expression
 * @param[in] defaultPort the default port if omitted
 * @return the hostname
 */
int get_port(const char *expression, int defaultPort) {
    int port;
    int pos;

    pos = strpos(expression, ":");
    if (pos == STR_NOT_FOUND) {
        return defaultPort;
    }

    expression += pos + 1;
    port = atoi(expression);
    if (port <= 0 || port > 65535) {
        // mq connect server: should use default port instead of 0
        // We ignore gracefully any other error
        return defaultPort;
    }

    return port;
}
