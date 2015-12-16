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

# Features

ENABLE_E4C=1

# Other options

CCOPTS=-DDEBUG -g

ifeq ($(ENABLE_E4C),1)
	CCOPTS+=-DUSE_E4C
endif

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

# Main build targets

all: print-banner lib pkg

clean: cleanlib cleanpkg

# Build steps targets

lib: ${BUILD}/rabbitmq.so

pkg: ${BUILD}/pkgIndex.tcl

cleanlib:
	$(RM) core ${BUILD}/*.o ${BUILD}/*.so ${BUILD}/*.so.${VERSION}

cleanpkg:
	$(RM) ${BUILD}/makePkgIndex ${BUILD}/pkgIndex.tcl

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
ifeq ($(ENABLE_E4C),1)
	${CC} -c -fPIC ../vendor/e4c/e4c.c -o ${BUILD}/e4c.o
endif
	${CC} -c -fPIC rabbitmq-tcl.c -o ${BUILD}/rabbitmq.o

${BUILD}/makePkgIndex:
	${MKDIR} ${BUILD}
	${CC} makePkgIndex.c -o ${BUILD}/makePkgIndex

${BUILD}/pkgIndex.tcl: ${BUILD}/makePkgIndex
	${BUILD}/makePkgIndex pkgIndex.tcl.in > ${BUILD}/pkgIndex.tcl
