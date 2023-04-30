#   -------------------------------------------------------------
#
#    ___  __ \_____ ___  /____  /____(_)_  /___   |/  /_  __ \
#    __  /_/ /  __ `/_  __ \_  __ \_  /_  __/_  /|_/ /_  / / /
#    _  _, _// /_/ /_  /_/ /  /_/ /  / / /_ _  /  / / / /_/ /
#    /_/ |_| \__,_/ /_.___//_.___//_/  \__/ /_/  /_/  \___\_\
#                                                        _____________________
#   RabbitMQ C AMQP client library TCL wrapper          ___  __/_  ____/__  /
#   TCL module to connect to AMQP brokers.              __  /  _  /    __  /
#                                                       _  /   / /___  _  /___
#                                                       /_/    \____/  /_____/
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#   Project:        Nasqueron
#   Description:    Build instructions for make
#   License:        BSD-2-Clause
#   Note:           This Makefile is compatible with BSD make.
#                   The src/Makefile currently requires GNU make.
#   -------------------------------------------------------------

BUILD=build/

DOXYGEN=doxygen
GNU_MAKE=gmake
MAKE=${GNU_MAKE}
RMDIR=rm -rf

#   -------------------------------------------------------------
#   Main targets
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

all: build doc

build: build-lib build-pkg

clean: clean-build clean-doc

#   -------------------------------------------------------------
#   Build targets
#
#   The src/ Makefile provides two parts of the build:
#     - lib: the TCL extension module
#     - pkg: to allow to use `package require` instead of `load`
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

build-lib: ${BUILD}/rabbitmq.so
build-pkg: ${BUILD}/pkgIndex.tcl

${BUILD}/rabbitmq.so:
	(cd src && $(MAKE) lib)

${BUILD}/pkgIndex.tcl:
	(cd src && $(MAKE) pkg)

#   -------------------------------------------------------------
#   Documentation targets
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

doc: doc/build/html/index.html

doc/build/html/index.html:
	${DOXYGEN}

#   -------------------------------------------------------------
#   Clean targets
#   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean-doc:
	${RMDIR} doc/build

clean-build:
	(cd src && ${MAKE) clean)
