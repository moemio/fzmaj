SHELL = /bin/sh


CC = g++
LIBS = 
CFLAGS = -g -O   
DEPFLAGS = -M

FLAG = $(CFLAGS) $(LIBS) $(DEPFLAGS)

OBJ = fzmaj.o error.o input.o memory.o main.o 

all : Maj

Maj: $(OBJ) Makefile
	$(CC) $(OBJ) $(CFLAGS) -o Maj

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.d *.o

