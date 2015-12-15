/*  -------------------------------------------------------------
    RabbitMQ TCL - Unit testing - Headers
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
    Filename:       test.h
    Created:        2015-12-12
    Licence:        BSD-2-Clause
    -------------------------------------------------------------    */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/*  -------------------------------------------------------------
    Utils
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

// Code to test

#include "../src/utils/netutils.h"
#include "../src/utils/netutils.c"
/* strutils.h is included by netutils.c */

// Tests

#include "netutils.test.c"
#include "strutils.test.c"

/*  -------------------------------------------------------------
    Extension
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

// Code to test

#include "../src/rabbitmq-tcl.h"
#include "../src/rabbitmq-tcl.c"

// Tests

#include "helper-functions.test.c"
