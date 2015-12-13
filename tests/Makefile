# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
#   ___  __ \_____ ___  /____  /____(_)_  /___   |/  /_  __ \                 #
#   __  /_/ /  __ `/_  __ \_  __ \_  /_  __/_  /|_/ /_  / / /                 #
#   _  _, _// /_/ /_  /_/ /  /_/ /  / / /_ _  /  / / / /_/ /                  #
#   /_/ |_| \__,_/ /_.___//_.___//_/  \__/ /_/  /_/  \___\_\                  #
#                                                      _____________________  #
#  RabbitMQ C AMQP client library TCL wrapper          ___  __/_  ____/__  /  #
#  TCL module to connect to AMQP brokers.              __  /  _  /    __  /   #
#                                                      _  /   / /___  _  /___ #
#  (c) 2015, Nasqueron, some rights reserved.          /_/    \____/  /_____/ #
#  Released under BSD-2-Clause license.                                       #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Useful targets:
# ---------------
#   Compile and run tests ........ make runtest
#   Clean, compile and run ....... make retest

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
# Codebase and build configuration                                            #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# 3rd party versions

TCLVERSION=8.6

# Headers to include

TCL_INCLUDE=/usr/include/tcl${TCLVERSION}
INCLUDES=-I$(TCL_INCLUDE)

# Libraries to use

LIBS=-lcmocka -ltcl -lrabbitmq

# Commands to use

CC=clang ${INCLUDES} ${LIBS}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
# Targets, build process utility steps                                        #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Main targets

all: runtest

runtest: test
	@echo [ Part I - C tests ]
	./test
	@echo [ Part II - TCL tests ]
	tclsh${TCLVERSION} tests.tcl

clean:
	rm -f test

# Secondary convenience targets

tests: test

retest: clean runtest

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
# Compile and link process                                                    #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

test:
	${CC} test.c -o test
