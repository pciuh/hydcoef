# Makefile compiling program, uff!!
#
# Author:  sEbastian.biElicki
#
#      dAnzig, 2024/11/02
CC=gcc

CFLAGS=-g1 -Wall -ggdb

OBJ = main.o adm_approx.o dam_approx.o print_coef.o hydro.o in_out_files.o

all:     calhyd

calhyd: $(OBJ)
	$(CC) $(CFLAGS) -o calhyd $(OBJ) -lm

main.o:  main.c func.h

adm_approx.o: adm_approx.c func.h
dam_approx.o: dam_approx.c func.h
print_coef.o: print_coef.c func.h
hydro.o: hydro.c func.h
in_out_files.o: in_out_files.c func.h

clean:
	rm -rf *.o