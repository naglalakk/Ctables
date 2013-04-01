# tests/Makefile

SHELL=		/bin/sh

INCDIR=		$(.CURDIR)/../src

CC=			gcc
CFLAGS=		-g -Wall -O2 -I$(INCDIR)
LDFLAGS=	-L$(.CURDIR)/../lib
LIBS=		-lctables

.SUFFIXES: .c.o

all: main testEx testTrans

main: main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LIBS)

testEx: testEx.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LIBS)

testTrans: testTrans.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	@rm -f $(.CURDIR)/*.BAK main testEx testTrans *.o

.PHONY: clean
