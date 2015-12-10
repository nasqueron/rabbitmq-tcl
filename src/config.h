/*  -------------------------------------------------------------
    RabbitMQ TCL - Configuration
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

/*  -------------------------------------------------------------
    Table of contents
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    :: Library configuration
    :: Broker configuration

*/

/*  -------------------------------------------------------------
    Library configuration
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

// How many commands supplementary mq<n> commands do you want?

#define MQ_COMMANDS_AMOUNT 10

/*  -------------------------------------------------------------
    Broker configuration
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

// If you use `.mq connect` without any parameter, where do you want to connect?

#define BROKER_HOST "localhost"
#define BROKER_PORT 5672
#define BROKER_USER "guest"
#define BROKER_PASS "guest"
#define BROKER_VHOST "/"

// The exchange to use to publish messages to queues
#define BROKER_DEFAULT_EXCHANGE "amq.direct"
