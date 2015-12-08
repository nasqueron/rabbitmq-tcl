MAJOR_VERSION=0
VERSION=0.1.0

TCL_INCLUDE=/usr/include/tcl8.6
RABBITMQ_INCLUDE=/usr/include
INCLUDES=-I$(TCL_INCLUDE) -I$(RABBITMQ_INCLUDE)

LIBS=-ltcl8.6

#CCOPTS=-DUSE_TCL_STUBS=1
CC=clang ${CCOPTS} ${INCLUDES} ${LIBS}

RM=rm -f

# Main targets

all: print-banner rabbitmq.so

clean:
	$(RM) core *.o *.so *.so.${VERSION}

demo:
	rlwrap tclsh

# Helpers
print-banner:
	@cat doc/banner
	@date

# Files targets

rabbitmq.so: rabbitmq.o
	${CC} -shared -Wl,-soname,rabbitmq.so.${MAJOR_VERSION} -o rabbitmq.so.${VERSION} rabbitmq.o
	ln -s rabbitmq.so.${VERSION} rabbitmq.so

rabbitmq.o:
	${CC} -c -fPIC rabbitmq-tcl.c -o rabbitmq.o
