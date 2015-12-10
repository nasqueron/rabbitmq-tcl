#
# Tests for RabbitMQ TCL
#

CC=clang -lcmocka

#
# Main targets
#

all: test

test:
	${CC} netutils-test.c -o test

clean:
	rm -f test

#
# Secondary convenience targets
#

tests: test

retest: clean all
	./test
