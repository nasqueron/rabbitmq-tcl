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

#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#ifdef USE_E4C
#include "../vendor/e4c/e4c.h"
#endif

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
    amqp_connection_state_t connection;
} brokerConnections[MQ_COMMANDS_AMOUNT];

/**
 * Represents the client data information for a command.
 */
struct command_context {
    int commandNumber;
};

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
    sprintf(versionString, "%s %s", PRODUCT_NAME, PRODUCT_VERSION);
    return versionString;
}

/**
 * Determines if the specified connection slot is currently connected
 *
 * @param connectionNumber the number of the connection to check
 * @return 1 if connected, 0 if disconnected
 */
int is_mq_connected(int connectionNumber) {
    return brokerConnections[connectionNumber].connected;
}

/**
 * Determines if the connection will probably be non blocking
 *
 * @return 1 if the operation should be non blocking ; 0 on block
 */
int amqp_will_be_non_blocking_operation(amqp_connection_state_t connection) {
    // See
    // http://lists.rabbitmq.com/pipermail/rabbitmq-discuss/2012-February/018188.html
    return amqp_frames_enqueued(connection) || amqp_data_in_buffer(connection);

    // TODO: at the stage, we can try a third solution: run amqp_get_sockfd()
    // to get the socket descriptor, then call select() or poll().
    // If the socket can be read, we can return true.
}

/**
 * Prints an error message as command result and notify TCL an error occured.
 *
 * @param[out] tclInterpreter The interpreter in which to set result
 * @param[in] error The error message
 * @return TCL_ERROR
 */
int tcl_error(Tcl_Interp *tclInterpreter, char *error) {
    Tcl_SetResult(tclInterpreter, error, TCL_STATIC);
    return TCL_ERROR;
}

#ifdef USE_E4C
/**
 * Prints an E4C exception message and notifies TCL an error occured.
 *
 * @param[out] tclInterpreter The interpreter in which to set result
 * @return TCL_ERROR
 */
int tcl_exception(Tcl_Interp *tclInterpreter) {
    const e4c_exception *exception = e4c_get_exception();
    return tcl_error(tclInterpreter, strdup(exception->message));
}
#endif

/**
 * Gets a broker error
 *
 * @param[in] connection The AMQP connection
 * @param[out] rcpReply The AMQP RPC reply
 * @return 1 if an error occured, 0 if not
 */
int amqp_get_error(amqp_connection_state_t connection,
                   amqp_rpc_reply_t *rpcReply) {
    amqp_rpc_reply_t reply = amqp_get_rpc_reply(connection);
    rpcReply = &reply;
    return reply.reply_type != AMQP_RESPONSE_NORMAL;
}

/**
 * Determines the AMQP error from the server RPC reply, prints an error message
 * based on this error and the specified context, then notify TCL an error
 *occured.
 *
 * @param[out] tclInterpreter The interpreter in which to set result
 * @param[in] errorContext The context of the error message, typically what were
 *done at the moment of the error
 * @param[in] rcpReply The AMQP RPC reply
 * @return TCL_ERROR
 */
int tcl_amqp_error(Tcl_Interp *tclInterpreter, const char *errorContext,
                   amqp_rpc_reply_t rpcReply, int connectionNumber) {
    char *error;

    if (rpcReply.reply_type == AMQP_RESPONSE_NORMAL) {
        // Not an error
        return TCL_OK;
    }

    error = malloc(1024 * sizeof(char));
    if (rpcReply.reply_type == AMQP_RESPONSE_NONE) {
        sprintf(error, "%s a broker error occurred, but with an unexpected RPC "
                       "reply type. Please report this bug as issue.",
                errorContext);
    } else if (rpcReply.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
        sprintf(error, "%s %s", errorContext,
                amqp_error_string2(rpcReply.library_error));
    } else if (rpcReply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
        if (rpcReply.reply.id == AMQP_CONNECTION_CLOSE_METHOD) {
            amqp_connection_close_t *m =
                (amqp_connection_close_t *)rpcReply.reply.decoded;
            sprintf(error,
                    "%s a server connection error %d occurred, message: %.*s",
                    errorContext, m->reply_code, (int)m->reply_text.len,
                    (char *)m->reply_text.bytes);
            // Marks the connection as disconnected
            brokerConnections[connectionNumber].connected = 0;
        } else if (rpcReply.reply.id == AMQP_CHANNEL_CLOSE_METHOD) {
            amqp_channel_close_t *m =
                (amqp_channel_close_t *)rpcReply.reply.decoded;
            sprintf(error, "%s a server channel error %d occurred: %.*s",
                    errorContext, m->reply_code, (int)m->reply_text.len,
                    (char *)m->reply_text.bytes);
            // Marks the connection as disconnected
            brokerConnections[connectionNumber].connected = 0;
        } else {
            sprintf(error,
                    "%s an unknown server error occurred, method id 0x%08X",
                    errorContext, rpcReply.reply.id);
        }
    }

    return tcl_error(tclInterpreter, error);
}

/*  -------------------------------------------------------------
    TCL commands

    mq
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

/**
 * mq command
 *
 * @param[in] clientData The context with the command number
 * @param[out] tclInterpreter The interpreter calling this function
 * @param[in] argc The amount of command arguments
 * @param[in] argv The command arguments
 * @return TCL_OK, TCL_ERROR, TCL_RETURN, TCL_BREAK or TCL_CONTINUE
 */
static int mq_command(ClientData clientData, Tcl_Interp *tclInterpreter,
                      int argc, Tcl_Obj *const argv[]) {
    int connectionNumber, result;
    char *command;
    struct command_context *context;

    // Parses arguments and prepares context

    if (argc < 2) {
        return mq_usage(tclInterpreter);
    }

    context = clientData;
    connectionNumber = context->commandNumber;
    command = Tcl_GetString(argv[1]);

    // Run commands

    argc -= 2;
    argv += 2;

#ifdef USE_E4C
    volatile int status = 1;
    const e4c_signal_mapping *old_mappings;

    e4c_reusing_context(status, 0) {

        old_mappings = e4c_context_get_signal_mappings();
        e4c_context_set_signal_mappings(e4c_default_signal_mappings);

        try {
#endif
            result = call_mq_command(connectionNumber, tclInterpreter, command,
                                     argc, argv);
#ifdef USE_E4C
        } catch (RuntimeException) {
            result = tcl_exception(tclInterpreter);
        }

        e4c_context_set_signal_mappings(old_mappings);
    }
#endif

    return result;
}

/**
 * Calls the relevant mq command and returns result
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1,
 *…)
 * @param[out] tclInterpreter The interpreter calling this function
 * @param[in] argc The amount of command arguments
 * @param[in] argv The command arguments
 * @return TCL_OK, TCL_ERROR, TCL_RETURN, TCL_BREAK or TCL_CONTINUE
 */
int call_mq_command(int connectionNumber, Tcl_Interp *tclInterpreter,
                    char *command, int argc, Tcl_Obj *const argv[]) {
    if (strcmp(command, "bindqueue") == 0) {
        return mq_bindqueue(connectionNumber, tclInterpreter, argc, argv);
    } else if (strcmp(command, "connect") == 0) {
        return mq_connect(connectionNumber, tclInterpreter, argc, argv);
    } else if (strcmp(command, "connected") == 0) {
        return mq_connected(connectionNumber, tclInterpreter);
    } else if (strcmp(command, "disconnect") == 0) {
        return mq_disconnect(connectionNumber, tclInterpreter);
    } else if (strcmp(command, "get") == 0) {
        return mq_get(connectionNumber, tclInterpreter, argc, argv);
    } else if (strcmp(command, "publish") == 0) {
        return mq_publish(connectionNumber, tclInterpreter, argc, argv);
    } else if (strcmp(command, "version") == 0) {
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
    return tcl_error(tclInterpreter, "Usage: mq "
                                     "<bindqueue|connect|connected|disconnect|"
                                     "get|publish|version>");
}

/**
 * mq version
 *
 * @param[out] tclInterpreter The interpreter calling this function
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
 * @param[out] tclInterpreter The interpreter calling this function
 * @param[in] argc The amount of command arguments
 * @param[in] argv The command arguments
 * @return TCL_OK on success, TCL_ERROR if already connected or can't connect
 */
int mq_connect(int connectionNumber, Tcl_Interp *tclInterpreter, int argc,
               Tcl_Obj *const argv[]) {

    char *host, *hostPortExpression, *user, *pass, *vhost;
    int port, status;
    amqp_connection_state_t conn;
    amqp_socket_t *socket;
    amqp_rpc_reply_t result;

    // We don't allow to reconnect without first used mq disconnect

    if (is_mq_connected(connectionNumber)) {
        return tcl_error(tclInterpreter, "Already connected.");
    }

    // Connection parameters

    if (argc > 0 && argv[0]) {
        hostPortExpression = Tcl_GetString(argv[0]);
        host = get_host(hostPortExpression, BROKER_HOST);
        port = get_port(hostPortExpression, BROKER_PORT);
    } else {
        host = BROKER_HOST;
        port = BROKER_PORT;
    }
    if (argc > 1 && argv[1]) {
        user = Tcl_GetString(argv[1]);
    } else {
        user = BROKER_USER;
    }
    if (argc > 2 && argv[2]) {
        pass = Tcl_GetString(argv[2]);
    } else {
        pass = BROKER_PASS;
    }
    if (argc > 3 && argv[3]) {
        vhost = Tcl_GetString(argv[3]);
    } else {
        vhost = BROKER_VHOST;
    }

    // Opens a TCP connection to the broker

    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        return tcl_error(tclInterpreter, "Can't create TCP socket.");
    }

    status = amqp_socket_open(socket, host, port);
    if (status) {
        return tcl_error(tclInterpreter, "Can't connect to the broker.");
    }

    // Logins to the broker
    // No heartbeat, unlimited channels, 128K (131072) frame size
    result = amqp_login(conn, vhost, 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, user,
                        pass);
    if (result.reply_type != AMQP_RESPONSE_NORMAL) {
        return tcl_error(tclInterpreter, "Can't login to the broker.");
    }

    // Opens a first channel
    amqp_channel_open(conn, 1);
    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't open a channel:", result,
                              connectionNumber);
    }

    // We're connected. All is good.
    brokerConnections[connectionNumber].connection = conn;
    brokerConnections[connectionNumber].connected = 1;
    return TCL_OK;
}

/**
 * mq disconnect
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1, …)
 * @param[out] tclInterpreter The interpreter calling this function
 * @return TCL_OK on success, TCL_ERROR if not connected
 */
int mq_disconnect(int connectionNumber, Tcl_Interp *tclInterpreter) {
    amqp_rpc_reply_t result;

    if (!is_mq_connected(connectionNumber)) {
        return tcl_error(tclInterpreter, "Not connected.");
    }

    // We mark early as disconnected, to allow to recycle the slot
    // event if an error occurs during disconnect.
    brokerConnections[connectionNumber].connected = 0;

    amqp_connection_state_t conn =
        brokerConnections[connectionNumber].connection;

    result = amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    if (result.reply_type != AMQP_RESPONSE_NORMAL) {
        return tcl_amqp_error(tclInterpreter,
                              "An error occured closing channel:", result,
                              connectionNumber);
    }
    result = amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    if (result.reply_type != AMQP_RESPONSE_NORMAL) {
        return tcl_amqp_error(tclInterpreter,
                              "An error occured closing connection:", result,
                              connectionNumber);
    }

    amqp_destroy_connection(conn);
    return TCL_OK;
}

/**
 * mq connected
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1, …)
 * @param[out] tclInterpreter The interpreter calling this function
 * @return TCL_OK
 */
int mq_connected(int connectionNumber, Tcl_Interp *tclInterpreter) {
    char *reply = malloc(2 * sizeof(char));
    sprintf(reply, "%d", is_mq_connected(connectionNumber));
    Tcl_SetResult(tclInterpreter, reply, TCL_STATIC);
    return TCL_OK;
}

/**
 * mq get <queue> [-noack]
 */
int mq_get(int connectionNumber, Tcl_Interp *tclInterpreter, int argc,
           Tcl_Obj *const argv[]) {
    char *queue;
    char *argument;
    char *error;
    char *messageBody;
    amqp_boolean_t noAck = 0;

    amqp_connection_state_t conn;
    amqp_rpc_reply_t result;
    amqp_message_t message;

    // Parses arguments
    if (argc == 0) {
        return tcl_error(tclInterpreter, "Required queue missing.");
    }
    queue = Tcl_GetString(argv[0]);

    if (argc > 1) {
        argument = Tcl_GetString(argv[1]);
        if (strcmp(argument, "-noack") == 0) {
            noAck = 1;
        } else {
            error = malloc(1024 * sizeof(char));
            sprintf(error, "Unknown argument: %s", argument);
            return tcl_error(tclInterpreter, error);
        }
    }
    if (argc > 2) {
        return tcl_error(tclInterpreter, "Too many arguments.");
    }

    // Ensures we're connected
    if (!is_mq_connected(connectionNumber)) {
        return tcl_error(tclInterpreter, "Not connected.");
    }
    conn = brokerConnections[connectionNumber].connection;

    // Gets message from specified queue (basic.get operation)
    result = amqp_basic_get(conn, 1, amqp_cstring_bytes(queue), noAck);
    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't get message from queue:",
                              result, connectionNumber);
    }

    if (!amqp_will_be_non_blocking_operation(conn)) {
        // We can't assert the operation will be non blocking
        return TCL_OK;
    }

    result = amqp_read_message(conn, 1, &message, 0);
    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't read message:", result,
                              connectionNumber);
    }

    if (&message && &(message.body)) {
        messageBody = malloc(message.body.len + 1024);
        sprintf(messageBody, "%.*s", (int)message.body.len,
                (char *)message.body.bytes);

        // Cleans up
        amqp_destroy_message(&message);
        amqp_maybe_release_buffers(conn);
    }

    // TCL return
    Tcl_SetResult(tclInterpreter, messageBody, TCL_STATIC);

    return TCL_OK;
}

/**
 * mq bindqueue <exchange> [binding key]
 */
int mq_bindqueue(int connectionNumber, Tcl_Interp *tclInterpreter, int argc,
                 Tcl_Obj *const argv[]) {
    char *exchange, *exchangeType, *bindingKey;
    amqp_bytes_t queueName;
    amqp_queue_declare_ok_t *queueResult;
    amqp_connection_state_t conn;
    amqp_rpc_reply_t result;

    // Parses arguments
    if (argc == 0) {
        return tcl_error(tclInterpreter, "Argument required: exchange.");
    }
    exchange = Tcl_GetString(argv[0]);

    if (argc > 1) {
        bindingKey = Tcl_GetString(argv[1]);
    } else {
        bindingKey = BROKER_DEFAULT_BINDING_KEY;
    }

    if (argc > 2) {
        return tcl_error(tclInterpreter, "Too many arguments.");
    }

    // Ensures we're connected
    if (!is_mq_connected(connectionNumber)) {
        return tcl_error(tclInterpreter, "Not connected.");
    }

    conn = brokerConnections[connectionNumber].connection;

    // Declares the exchange
    exchangeType = BROKER_DEFAULT_EXCHANGE_TYPE;
    amqp_exchange_declare(conn, 1, amqp_cstring_bytes(exchange),
                          amqp_cstring_bytes(exchangeType), 0, 0, 0, 0,
                          amqp_empty_table);

    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't declare exchange:", result,
                              connectionNumber);
    }

    // Declares a new temporary queue
    queueResult = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0, 0, 0, 1,
                                     amqp_empty_table);
    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't declare queue:", result,
                              connectionNumber);
    }
    queueName = amqp_bytes_malloc_dup(queueResult->queue);
    if (queueName.bytes == NULL) {
        return tcl_error(tclInterpreter,
                         "Out of memory while copying queue name.");
    }

    // Binds it to the specified exchange
    amqp_queue_bind(conn, 1, queueName, amqp_cstring_bytes(exchange),
                    amqp_cstring_bytes(bindingKey), amqp_empty_table);
    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't bind queue:", result,
                              connectionNumber);
    }

    // Stores queue name as result
    Tcl_SetObjResult(tclInterpreter,
                     Tcl_NewStringObj(queueName.bytes, queueName.len));
    return TCL_OK;
}

/**
 *
 * mq publish <target> [-key <routing key>] [-mandatory] [-immediate] <message>
 *
 * The target could be: <exchange>, -exchange <exchange> or -queue <queue>
 *
 * @param[in] connectionNumber The connection offset (0 for mq, 1 for mq1, …)
 * @param[out] tclInterpreter The interpreter calling this function
 * @param[in] argc The amount of command arguments
 * @param[in] argv The command arguments
 * @return TCL_OK on success, TCL_ERROR if already connected or can't connect
 */
int mq_publish(int connectionNumber, Tcl_Interp *tclInterpreter, int argc,
               Tcl_Obj *const argv[]) {
    int i;
    amqp_connection_state_t conn;
    amqp_rpc_reply_t result;
    char *argument, *exchange, *routingKey, *content;
    int contentParsed = 0;
    int targetParsed = 0;
    int routingKeyParsed = 0;
    int mustRouteToQueue = 0;                 // -mandatory
    int mustImmediatelyDeliverToConsumer = 0; // -immediate

    // Parses arguments
    for (i = 0; i < argc; i++) {
        argument = Tcl_GetString(argv[i]);
        if (strcmp(argument, "-key") == 0) {
            if (argc < i + 2) {
                return tcl_error(tclInterpreter,
                                 "Required routing key argument missing.");
            }
            routingKey = Tcl_GetString(argv[++i]);
            routingKeyParsed = 1;
        } else if (strcmp(argument, "-mandatory") == 0) {
            mustRouteToQueue = 1;
        } else if (strcmp(argument, "-immediate") == 0) {
            mustImmediatelyDeliverToConsumer = 1;
        } else if (strcmp(argument, "-exchange") == 0) {
            if (argc < i + 2) {
                return tcl_error(tclInterpreter,
                                 "Required exchange argument missing.");
            }
            exchange = Tcl_GetString(argv[++i]);
            targetParsed = 1;
        } else if (strcmp(argument, "-queue") == 0) {
            if (argc < i + 2) {
                return tcl_error(tclInterpreter,
                                 "Required queue argument missing.");
            }
            exchange = BROKER_DEFAULT_EXCHANGE;
            routingKey = Tcl_GetString(argv[++i]);
            targetParsed = 1;
            routingKeyParsed = 1;
        } else if (i == 0) {
            exchange = Tcl_GetString(argv[0]);
            targetParsed = 1;
        } else {
            content = Tcl_GetString(argv[i]);
            contentParsed = 1;
        }
    }

    if (!routingKeyParsed) {
        routingKey = BROKER_DEFAULT_ROUTING_KEY;
    }

    // Throws error when we haven't enough information (what and where)
    if (!targetParsed) {
        return tcl_error(tclInterpreter, "Required message target missing.");
    }

    if (!contentParsed) {
        return tcl_error(tclInterpreter,
                         "Required message content argument missing.");
    }

    // Ensures we're connected
    if (!is_mq_connected(connectionNumber)) {
        return tcl_error(tclInterpreter, "Not connected.");
    }

    // Sends the message and checks the result
    conn = brokerConnections[connectionNumber].connection;
    amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchange),
                       amqp_cstring_bytes(routingKey), mustRouteToQueue,
                       mustImmediatelyDeliverToConsumer, NULL,
                       amqp_cstring_bytes(content));

    if (amqp_get_error(conn, &result)) {
        return tcl_amqp_error(tclInterpreter, "Can't publish message:", result,
                              connectionNumber);
    }

#ifdef DEBUG
    char *debugMessage = malloc(1024 * sizeof(char) + sizeof(content));
    sprintf(
        debugMessage,
        "Message published to exchange '%s', routing key '%s'. Content: '%s'.",
        exchange, routingKey, content);
    Tcl_SetResult(tclInterpreter, debugMessage, TCL_STATIC);
#endif

    return TCL_OK;
}

/*  -------------------------------------------------------------
    TCL initialisation code

    Creation of the mq and mq2-10 commands.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    */

#ifdef WINDOWS
extern __declspec(dllexport) int Rabbitmq_Init(Tcl_Interp *tclInterpreter);
#else
extern int Rabbitmq_Init(Tcl_Interp *tclInterpreter);
#endif

/**
 * Determines TCL environment is correct and initiliazes stubs if needed
 *
 * @param[out] tclInterpreter The current TCL interpreter
 * @return 1 on success; otherwise, 0
 */
int tcl_init(Tcl_Interp *tclInterpreter) {
    // This extension requires TCL 8.0+
    if (Tcl_PkgRequire(tclInterpreter, "Tcl", TCL_VERSION, 0) == NULL) {
        if (TCL_VERSION[0] == '7') {
            if (Tcl_PkgRequire(tclInterpreter, "Tcl", "8.0", 0) == NULL) {
                return 0;
            }
        }
    }

#if USE_TCL_STUBS
    // Initializes stubs (requires TCL 8.1+)
    if (Tcl_InitStubs(tclInterpreter, "8.1", 0) == NULL) {
        return 0;
    }
#endif

    return 1;
}

/**
 * Gets command context
 *
 * @param commandNumber The command number (3 for mq3)
 * @return the command context
 */
ClientData get_mq_command_context(int commandNumber) {
    struct command_context *context = malloc(sizeof(int));
    context->commandNumber = commandNumber;
    return context;
}

/**
 * Creates TCL commands
 *
 * @param[out] tclInterpreter The current TCL interpreter
 */
void tcl_create_commands(Tcl_Interp *tclInterpreter) {
    int i;
    char commandName[10];

    // Creates mq and mq1-mq10 commands
    for (i = 0; i <= MQ_COMMANDS_AMOUNT; i++) {
        if (i == 0) {
            strcpy(commandName, "mq");
        } else {
            sprintf(commandName, "mq%d", i);
        }

        Tcl_CreateObjCommand(tclInterpreter, commandName, mq_command,
                             get_mq_command_context(i),
                             (Tcl_CmdDeleteProc *)NULL);

        brokerConnections[i].connected = 0;
    }
}

/**
 * Provides TCL package
 *
 * @param[out] tclInterpreter The current TCL interpreter
 */
int tcl_provide_package(Tcl_Interp *tclInterpreter) {
    int result;

    result = Tcl_PkgProvide(tclInterpreter, PACKAGE_NAME, PACKAGE_VERSION);
    return (result != TCL_ERROR);
}

/**
 * Initializes the library and provide mq commands
 *
 * @param[out] tclInterpreter The current TCL interpreter to provide commands to
 * @return TCL_OK on success; otherwise, TCL_ERROR
 */
int Rabbitmq_Init(Tcl_Interp *tclInterpreter) {
    if (!tcl_init(tclInterpreter)) {
        return TCL_ERROR;
    }

    tcl_create_commands(tclInterpreter);

    if (!tcl_provide_package(tclInterpreter)) {
        return TCL_ERROR;
    }

    return TCL_OK;
}
