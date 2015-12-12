/*  -------------------------------------------------------------
    RabbitMQ TCL - Unit testing - RabbitMQ helper functions
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
    Filename:       helper-functions.test.c
    Created:        2015-12-12
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

/*  -------------------------------------------------------------
    Broker helper functions

    int is_mq_connected(int connectionNumber)
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

static void test_is_mq_connected(void **state) {
    assert_int_equal(0, is_mq_connected(0));

    brokerConnections[1].connected = 1; // TODO: mock mq_connect
    assert_int_equal(0, is_mq_connected(0));
    assert_int_equal(1, is_mq_connected(1));

    brokerConnections[1].connected = 0; // TODO: mock mq_disconnect
    assert_int_equal(0, is_mq_connected(0));
    assert_int_equal(0, is_mq_connected(1));
}
