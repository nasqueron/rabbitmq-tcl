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
#   Compile released version ........... make
# 	Compile customized build ........... make clean lint all
# 	Compile, run tests & tclsh ......... make rebuild

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
# Codebase and build configuration                                            #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Library versioning
# These variables should be coherent with src/version.h

MAJOR_VERSION=0
VERSION=0.1.0

# 3rd party versions

TCLVERSION=8.6

# Headers to include

TCL_INCLUDE=/usr/include/tcl${TCLVERSION}
RABBITMQ_INCLUDE=/usr/include
INCLUDES=-I$(TCL_INCLUDE) -I$(RABBITMQ_INCLUDE)

# Libraries to use

LIBS=-ltcl${TCLVERSION} -lrabbitmq

# Other options
CCOPTS=-DDEBUG

# Paths

BUILD=../build

# Commands to use

ARC=arc
CAT=cat
CC=clang ${CCOPTS} ${INCLUDES}
DATE=date
LINKER=clang ${LIBS}
MKDIR=mkdir -p
RLWRAP=rlwrap
RM=rm -f
SYMLINK=ln -s
TCLSH=tclsh${TCLVERSION}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
# Targets, build process utility steps                                        #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Main targets

all: print-banner ${BUILD}/rabbitmq.so

clean:
	$(RM) core ${BUILD}/*.o ${BUILD}/*.so ${BUILD}/*.so.${VERSION}

# Dev targets

lint:
	${ARC} lint --apply-patches

test:
	cd ../tests && make retest

demo:
	${RLWRAP} ${TCLSH}

rebuild: clean lint all test demo

# Helpers

print-banner:
	@${CAT} ../doc/banner
	@${DATE}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
# Compile and link process                                                    #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

${BUILD}/rabbitmq.so: ${BUILD}/rabbitmq.o
	${LINKER} -shared -Wl,-soname,rabbitmq.so.${MAJOR_VERSION} -o ${BUILD}/rabbitmq.so.${VERSION} ${BUILD}/*.o
	cd ${BUILD} && ${SYMLINK} rabbitmq.so.${VERSION} rabbitmq.so

${BUILD}/rabbitmq.o:
	${MKDIR} ${BUILD}
	${CC} -c -fPIC utils/netutils.c -o ${BUILD}/netutils.o
	${CC} -c -fPIC rabbitmq-tcl.c -o ${BUILD}/rabbitmq.o
