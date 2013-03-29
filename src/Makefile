# src/Makefile

SHELL=	/bin/sh

AR=		ar
AROPTS=	rcs

LIB=	Ctables.a
OBJ=	Ctables.o
DEPS=	Ctables.h

CC=	cc
CFLAGS=	-O2 -std=c99 -g -Wall -Werror -pedantic

.SUFFIXES: .c.o
.PHONY: clean

$(LIB): $(OBJ)
	$(AR) $(AROPTS) $(LIB) $(OBJ)

.c.o: $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -f $(.CURDIR)/*.BAK *.o $(LIB)
