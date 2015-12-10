/*  -------------------------------------------------------------
    RabbitMQ TCL - Unit testing - Net helper functions
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
    Filename:       config.h
    Created:        2015-12-08
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/utils/netutils.h"
#include "../src/utils/netutils.c"

/*  -------------------------------------------------------------
    get_host, get_port

    char *get_host(const char *expression, const char *defaultHost);
    int get_port(const char *expression, int defaultPort);
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

static void test_get_host(void **state) {
    assert_string_equal("alpha", get_host("alpha:5000", "localhost"));
    assert_string_equal("beta", get_host("beta", "localhost"));
    assert_string_equal("delta", get_host("", "delta"));
    assert_string_equal("gamma", get_host(":5000", "gamma"));
}

static void test_get_port(void **state) {
    assert_int_equal(5000, get_port("alpha:5000", 1234));
    assert_int_equal(1234, get_port("beta", 1234));
    assert_int_equal(5000, get_port(":5000", 1234));
    assert_int_equal(1234, get_port("", 1234));
}

/*  -------------------------------------------------------------
    Test entry point
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_get_host),
        unit_test(test_get_port),
    };
    return run_tests(tests);
}
