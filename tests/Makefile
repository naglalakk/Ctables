# tests/Makefile

SHELL=		/bin/sh

INCDIR=		$(.CURDIR)/../include

CC=			cc
CFLAGS=		-g -Wall -O2 -I$(INCDIR)
LDFLAGS=	-L$(.CURDIR)/../lib/
LIBS=		$(.CURDIR)/../lib/libctables.a

.SUFFIXES: .c.o

all: main testEx testTrans

main: main.o
	$(CC) -o $@ $(LDFLAGS) $(LIBS) main.o

testEx: testEx.o
	$(CC) -o $@ $(LDFLAGS) $(LIBS) testEx.o

testTrans: testTrans.o
	$(CC) -o $@ $(LDFLAGS) $(LIBS) testTrans.o

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -f $(.CURDIR)/*.BAK *.o $(PROG)

.PHONY: clean
