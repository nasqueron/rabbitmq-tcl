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
    Filename:       strpos.test
    Created:        2015-12-12
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

/*  -------------------------------------------------------------
    strutils

    int strpos(const char *haystack, const char *needle)
    char *str_replace(const char *needle, const char *replace, *haystack)
    STR_NOT_FOUND
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

static void test_strpos(void **state) {
    assert_int_equal(STR_NOT_FOUND, strpos("alpha:5000", "1234"));
    assert_int_equal(0, strpos("alpha:5000", "alpha"));
    assert_int_equal(5, strpos("alpha:5000", ":"));

    assert_int_equal(STR_NOT_FOUND, strpos("", "foo"));
    assert_int_equal(0, strpos("foo", ""));
}

static void test_str_replace(void **state) {
    assert_string_equal("alpha:1234",
                        str_replace("5000", "1234", "alpha:5000"));
    assert_string_equal("alpha:1234", str_replace("", "", "alpha:1234"));
    assert_string_equal("alpha", str_replace(":1234", "", "alpha:1234"));
    assert_string_equal("", str_replace("5000", "1234", ""));
    assert_string_equal("", str_replace("", "", ""));
}
