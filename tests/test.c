/*  -------------------------------------------------------------
    RabbitMQ TCL - Unit testing - String helper functions
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
    Filename:       test.c
    Created:        2015-12-12
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test.h"

/*  -------------------------------------------------------------
    Test entry point
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

int main(void) {
    const UnitTest tests[] = {
        // netutils
        unit_test(test_get_host), unit_test(test_get_port),
    };
    return run_tests(tests);
}
