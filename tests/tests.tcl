#   -------------------------------------------------------------
#   RabbitMQ TCL - Unit testing
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
#    ___  __ \_____ ___  /____  /____(_)_  /___   |/  /_  __ \
#    __  /_/ /  __ `/_  __ \_  __ \_  /_  __/_  /|_/ /_  / / /
#    _  _, _// /_/ /_  /_/ /  /_/ /  / / /_ _  /  / / / /_/ /
#    /_/ |_| \__,_/ /_.___//_.___//_/  \__/ /_/  /_/  \___\_\
#                                                       _____________________
#   RabbitMQ C AMQP client library TCL wrapper          ___  __/_  ____/__  /
#   TCL module to connect to AMQP brokers.              __  /  _  /    __  /
#                                                       _  /   / /___  _  /___
#   (c) 2015, Nasqueron, some rights reserved.          /_/    \____/  /_____/
#   Released under BSD-2-Clause license.
#
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#   Software:       RabbitMQ TCL
#   Author:         Sébastien Santoro aka Dereckson
#   Filename:       tests.tcl
#   Created:        2015-12-12
#   Licence:        BSD-2-Clause
#   -------------------------------------------------------------

load ../build/rabbitmq.so

package require rabbitmq
package require tcltest
namespace import ::tcltest::*

test mq-connected {When the interpreter starts, we shouldn't be marked as connected to the broker.} {
    mq connected
} 0
