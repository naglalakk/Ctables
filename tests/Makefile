# tests/Makefile

SHELL=		/bin/sh

INCDIR=		$(.CURDIR)/../include
DEPS=		$(INCDIR)/Ctables.h

CC=			cc
CFLAGS=		-I$(INCDIR) -O0 -std=c99 -g -Wall -Werror -pedantic
LDFLAGS=	-L$(.CURDIR)/../lib/
LIBS=		-lctables

.SUFFIXES: .c.o
.PHONY: clean

all: main testEx testTrans

main: main.o
	$(CC) -o $@ $(LDFLAGS) $(LIBS) main.o

testEx: testEx.o
	$(CC) -o $@ $(LDFLAGS) $(LIBS) testEx.o

testTrans: testTrans.o
	$(CC) -o $@ $(LDFLAGS) $(LIBS) testTrans.o

.c.o: $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -f $(.CURDIR)/*.BAK *.o $(PROG)
