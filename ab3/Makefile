CC=g++
CCFLAGS=-c -Wall -Wextra -ansi -O3 -ffast-math -I mongoose/
LD=g++
LDFLAGS=-Lmongoose/ -lmongoose

all: main.o node.o graph.o utils.o
	$(LD) $(LDFLAGS) main.o node.o graph.o utils.o -o ab3

main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp

node.o: node.cpp
	$(CC) $(CCFLAGS) node.cpp

graph.o: graph.cpp
	$(CC) $(CCFLAGS) graph.cpp

utils.o: utils.cpp
	$(CC) $(CCFLAGS) utils.cpp

clean:
	rm -f *.o
	rm -f ab3
