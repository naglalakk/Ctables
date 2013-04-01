# src/Makefile
# This makefile is sufficient to build the library
# containing Ctables.o object file.

SHELL=	/bin/sh

AR=		ar
AROPTS=	rcs

LIB=	$(.CURDIR)/../lib/libctables.a
SRC=	Ctables.c
OBJ=	${SRC:.c=.o}

CC=		cc
CFLAGS=	-g -Wall -O2

.SUFFIXES: .c.o

$(LIB): $(OBJ)
	$(AR) $(AROPTS) $(LIB) $(OBJ) 

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@echo Removing object files and archive.
	@rm -f $(.CURDIR)/*.BAK src/*.o $(LIB)

.PHONY: clean
