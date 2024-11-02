# Makefile compiling program, uff!!
#
# Author:  sEbastian.biElicki
#
#      dAnzig, 2024/11/02
CC=gcc
CPP=g++
FDIR=functions/

CFLAGS=-g3 -Wall -ggdb

OBJ = main.o adm_approx.o dam_approx.o

all:     calhyd

calhyd: $(OBJ)
	$(CC) $(CFLAGS) -o calhyd $(OBJ)

main.o:  main.c func.h

adm_approx.o: adm_approx.c func.h
dam_approx.o: dam_approx.c func.h

clean:
	rm -rf *.o