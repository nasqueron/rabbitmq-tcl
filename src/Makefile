

MAJOR_VERSION=0
VERSION=0.1.0

TCL_INCLUDE=/usr/include/tcl8.6
RABBITMQ_INCLUDE=/usr/include
INCLUDES=-I$(TCL_INCLUDE) -I$(RABBITMQ_INCLUDE)

BUILD=../build
LIBS=-ltcl8.6 -lrabbitmq

#CCOPTS=-DUSE_TCL_STUBS=1
CC=clang ${CCOPTS} ${INCLUDES}
LINKER=clang ${LIBS}

RM=rm -f

# Main targets

all: print-banner ${BUILD}/rabbitmq.so

clean:
	$(RM) core ${BUILD}/*.o ${BUILD}/*.so ${BUILD}/*.so.${VERSION}

# Dev targets

lint:
	arc lint --apply-patches

demo:
	rlwrap tclsh

rebuild: clean lint all demo

# Helpers

print-banner:
	@cat ../doc/banner
	@date

# Files targets

${BUILD}/rabbitmq.so: ${BUILD}/rabbitmq.o
	${LINKER} -shared -Wl,-soname,rabbitmq.so.${MAJOR_VERSION} -o ${BUILD}/rabbitmq.so.${VERSION} ${BUILD}/*.o
	cd ${BUILD} && ln -s rabbitmq.so.${VERSION} rabbitmq.so

${BUILD}/rabbitmq.o:
	mkdir -p ${BUILD}
	${CC} -c -fPIC utils/netutils.c -o ${BUILD}/netutils.o
	${CC} -c -fPIC rabbitmq-tcl.c -o ${BUILD}/rabbitmq.o
