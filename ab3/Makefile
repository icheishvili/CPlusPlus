CC=g++
CCFLAGS=-c -Wall -Wextra -ansi -O3
LD=g++
LDFLAGS=

all: main.o
	$(LD) $(LDFLAGS) main.o -o ab3

main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp

clean:
	rm -f *.o
	rm -f ab3
