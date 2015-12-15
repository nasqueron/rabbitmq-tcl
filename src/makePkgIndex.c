/*  -------------------------------------------------------------
    RabbitMQ TCL - Create pkgIndex.tcl file
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
    Filename:       rabbitmq-tcl.h
    Created:        2015-12-08
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "version.h"
#include "utils/strutils.h"

/*  -------------------------------------------------------------
    Templates functions
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * Substitutes templates variables
 *
 * @param text The text with variables to substitute
 * @return The substituted text
 */
char *subst_template_variables(char *text) {
    char *substText;

    substText = str_replace("%%PACKAGE_NAME%%", PACKAGE_NAME, text);
    substText = str_replace("%%PACKAGE_VERSION%%", PACKAGE_VERSION, substText);

    return substText;
}

/**
 * Prints a template, after variable substitution
 *
 * @param filename The patho to the template
 */
void print_template(const char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Reads line by line, substitutes and prints
    // TODO: replace by more standard than getline
    fp = fopen(filename, "r");
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", subst_template_variables(line));
    }
    fclose(fp);
}

/*  -------------------------------------------------------------
    Program entry point
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

int main(int argc, char **argv) {
    char *filename;

    // Usage: <executable> <template file>
    if (argc != 2) {
        printf("Wrong argument count.\n");
        return 1;
    }

    // Template to read
    filename = argv[1];
    if (access(filename, F_OK) == -1) {
        return 2;
    }

    // Output
    print_template(filename);

    return 0;
}
