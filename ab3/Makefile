CC=g++
CCFLAGS=-c -Wall -Wextra -ansi -O3 -ffast-math -I mongoose/
LD=g++
LDFLAGS=-Lmongoose/ -lmongoose

all: main.o
	$(LD) $(LDFLAGS) main.o -o ab3

main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp

clean:
	rm -f *.o
	rm -f ab3
