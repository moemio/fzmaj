SHELL = /bin/sh


CC = g++
LIBS = 
CFLAGS = -g -O 
DEPFLAGS = -M

FLAG = $(CFLAGS) $(LIBS) $(DEPFLAGS)


CUR_PATH = ./
OBJ_PATH = ./
ALL_CPP = $(foreach dir, $(CUR_PATH),$(wildcard $(dir)*.cpp))
OBJ = $(patsubst $(CUR_PATH)%.cpp,%.o,$(ALL_CPP))

all : Maj 

Maj: $(OBJ)
	$(CC) *.o $(CFLAGS) -o Maj

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o

