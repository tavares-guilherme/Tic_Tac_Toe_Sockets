CC = g++

all: match.o server.o
	$(CC) match.o server.o -o server

match.o: match.cpp
	$(CC) match.cpp -c

server.o: server.cpp
	$(CC) server.cpp -c