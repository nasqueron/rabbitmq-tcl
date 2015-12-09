/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   ___  __ \_____ ___  /____  /____(_)_  /___   |/  /_  __ \
 *   __  /_/ /  __ `/_  __ \_  __ \_  /_  __/_  /|_/ /_  / / /
 *   _  _, _// /_/ /_  /_/ /  /_/ /  / / /_ _  /  / / / /_/ /
 *   /_/ |_| \__,_/ /_.___//_.___//_/  \__/ /_/  /_/  \___\_\
 *                                                      _____________________
 *  RabbitMQ C AMQP client library TCL wrapper          ___  __/_  ____/__  /
 *  TCL module to connect to AMQP brokers.              __  /  _  /    __  /
 *                                                      _  /   / /___  _  /___
 *  (c) 2015, Nasqueron, some rights reserved.          /_/    \____/  /_____/
 *  Released under BSD-2-Clause license.
 *
 *  Provides a TCL rabbitmq.so module to get a 'mq' TCL command.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <string.h>

#include <tcl.h>
/* TODO: include librabbitmq */

#include "config.h"
#include "version.h"
#include "rabbitmq-tcl.h"

/*  -------------------------------------------------------------
    Broker connections
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * Represents a connection with the broker, and stores properties.
 */
struct broker_connection {
    int connected; /* 0 if disconnected, 1 if connected */
} brokerConnections[MQ_COMMANDS_AMOUNT];

/*  -------------------------------------------------------------
    Helper functions
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * Gets product version string
 *
 * @return The product name and version
 */
char *get_version_string() {
    char *versionString = malloc(256 * sizeof(char));
    sprintf(versionString, "%s %s", RABBITMQ_TCL_PRODUCT_NAME,
            RABBITMQ_TCL_VERSION);
    return versionString;
}

/*  -------------------------------------------------------------
    TCL commands

    mq
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * mq command
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1, …)
 * @param[out] tclInterpreter The interpreter in which to create new command
 * @param[in] argc The amount of command arguments
 * @param[in] argv The command arguments
 * @return TCL_OK, TCL_ERROR, TCL_RETURN, TCL_BREAK or TCL_CONTINUE
 */
static int mq_command(ClientData clientData, Tcl_Interp *tclInterpreter,
                      int argc, char **argv) {
    if (argc <= 1) {
        return mq_usage(tclInterpreter);
    }

    int connectionNumber = (int)clientData;

    if (strcmp(argv[1], "connect") == 0) {
        return mq_connect(connectionNumber, tclInterpreter, argc - 2, argv + 2);
    } else if (strcmp(argv[1], "disconnect") == 0) {
        return mq_disconnect(connectionNumber, tclInterpreter);
    } else if (strcmp(argv[1], "version") == 0) {
        return mq_version(tclInterpreter);
    } else {
        return mq_usage(tclInterpreter);
    }
}

/**
 * mq [not existing command] usage output
 *
 * @param[out] tclInterpreter The interpreter to send command result to
 */
int mq_usage(Tcl_Interp *tclInterpreter) {
    Tcl_SetResult(tclInterpreter, "Usage: mq <connect|disconnect|version>",
                  TCL_STATIC);
    return TCL_ERROR;
}

/**
 * mq version
 *
 * @param[out] tclInterpreter The interpreter in which to create new command
 * @return TCL_OK
 */
int mq_version(Tcl_Interp *tclInterpreter) {
    Tcl_SetResult(tclInterpreter, get_version_string(), TCL_STATIC);
    return TCL_OK;
}

/**
 * mq connect
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1, …)
 * @param[out] tclInterpreter The interpreter in which to create new command
 * @param[in] argc The amount of command arguments
 * @param[in] argv The command arguments
 * @return TCL_OK on success, TCL_ERROR if already connected or can't connect
 */
int mq_connect(int connectionNumber, Tcl_Interp *tclInterpreter, int argc,
               char **argv) {

    char *host, *user, *pass, *vhost;
    int port;

    if (brokerConnections[connectionNumber].connected == 1) {
        Tcl_SetResult(tclInterpreter, "Already connected.", TCL_STATIC);
        return TCL_ERROR;
    }

    // Connection parameters

    if (argc > 0 && argv[0]) {
        host = get_host(argv[0], BROKER_HOST);
        port = get_port(argv[0], BROKER_PORT);
    } else {
        host = BROKER_HOST;
        port = BROKER_PORT;
    }
    if (argc > 1 && argv[1]) {
        user = argv[1];
    } else {
        user = BROKER_USER;
    }
    if (argc > 2 && argv[2]) {
        pass = argv[2];
    } else {
        pass = BROKER_PASS;
    }
    if (argc > 3 && argv[3]) {
        vhost = argv[3];
    } else {
        vhost = BROKER_VHOST;
    }

    char *debugInformation = malloc(256 * sizeof(char));
    //"Connecting to amqp://%s:%s@%s:%d"
    sprintf(debugInformation, "Connecting to amqp://%s:%s@%s:%d", user, pass,
            host, port);
    Tcl_SetResult(tclInterpreter, debugInformation, TCL_STATIC);

    if (0) {
        Tcl_SetResult(tclInterpreter, "Can't connect to the broker.",
                      TCL_STATIC);
        return TCL_ERROR;
    }

    brokerConnections[connectionNumber].connected = 1;
    return TCL_OK;
}

/**
 * mq disconnect
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1, …)
 * @param[out] tclInterpreter The interpreter in which to create new command
 * @return TCL_OK on success, TCL_ERROR if not connected
 */
int mq_disconnect(int connectionNumber, Tcl_Interp *tclInterpreter) {
    if (brokerConnections[connectionNumber].connected == 0) {
        Tcl_SetResult(tclInterpreter, "not connected", TCL_STATIC);
        return TCL_ERROR;
    }

    brokerConnections[connectionNumber].connected = 0;
    return TCL_OK;
}

/*  -------------------------------------------------------------
    TCL initialisation code

    Creation of the mq and mq2-10 commands.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

#ifdef WINDOWS
extern __declspec(dllexport) int mq_init(Tcl_Interp *tclInterpreter);
#else
extern int mq_init(Tcl_Interp *tclInterpreter);
#endif

/**
 * Initializes the library and provide mq commands
 *
 * @param[out] tclInterpreter The current TCL interpreter to provide command to
 * @return TCL_OK on success; otherwise, TCL_ERROR
 */
int Rabbitmq_Init(Tcl_Interp *tclInterpreter) {
    int i, result;
    char commandName[10];

    // TCL requirements and stubs

    if (Tcl_PkgRequire(tclInterpreter, "Tcl", TCL_VERSION, 0) == NULL) {
        if (TCL_VERSION[0] == '7') {
            if (Tcl_PkgRequire(tclInterpreter, "Tcl", "8.0", 0) == NULL) {
                return TCL_ERROR;
            }
        }
    }

#if USE_TCL_STUBS
    if (Tcl_InitStubs(tclInterpreter, "8.1", 0) == NULL) {
        return TCL_ERROR;
    }
#endif

    // Creates mq annd mq1-mq10 commands

    for (i = 0; i <= MQ_COMMANDS_AMOUNT; i++) {
        if (i == 0) {
            strcpy(commandName, "mq");
        } else {
            sprintf(commandName, "mq%d", i);
        }

        Tcl_CreateCommand(tclInterpreter, commandName, mq_command,
                          (ClientData)i, (Tcl_CmdDeleteProc *)NULL);

        brokerConnections[i].connected = 0;
    }

    // Provides a TCL package

    result = Tcl_PkgProvide(tclInterpreter, "rabbitmq", RABBITMQ_TCL_VERSION);
    if (result == TCL_ERROR) {
        return TCL_ERROR;
    }
    return TCL_OK;
}
