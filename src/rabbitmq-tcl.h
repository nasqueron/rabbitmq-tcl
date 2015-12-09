/*  -------------------------------------------------------------
    RabbitMQ TCL - Header
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

/*  -------------------------------------------------------------
    Functions from utils/
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

#include "utils/netutils.h"

/*  -------------------------------------------------------------
    Functions explicit declaration (by alphabetical order)
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

char *get_version_string();

static int mq_command(ClientData clientData, Tcl_Interp *tclInterpreter,
                      int argc, char **argv);
int mq_connect(int connectionNumber, Tcl_Interp *tclInterpreter, int argc,
               char **argv);
int mq_disconnect(int connectionNumber, Tcl_Interp *tclInterpreter);
int mq_usage(Tcl_Interp *tclInterpreter);
int mq_version(Tcl_Interp *tclInterpreter);

int Rabbitmq_Init(Tcl_Interp *tclInterpreter);
