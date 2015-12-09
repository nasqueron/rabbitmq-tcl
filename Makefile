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

all: print-banner build/rabbitmq.so

clean:
	$(RM) core build/*.o build/*.so build/*.so.${VERSION}

# Dev targets

lint:
	arc lint --apply-patches

demo:
	rlwrap tclsh

rebuild: clean lint all demo

# Helpers

print-banner:
	@cat doc/banner
	@date

# Files targets

build/rabbitmq.so: build/rabbitmq.o
	${CC} -shared -Wl,-soname,rabbitmq.so.${MAJOR_VERSION} -o build/rabbitmq.so.${VERSION} build/*.o
	cd build && ln -s rabbitmq.so.${VERSION} rabbitmq.so

build/rabbitmq.o:
	mkdir -p build
	${CC} -c -fPIC rabbitmq-tcl.c -o build/rabbitmq.o
