CC = g++

all: match.o client.o
	$(CC) match.o client.o -o client

match.o: match.cpp
	$(CC) match.cpp -c

client.o: client.cpp
	$(CC) client.cpp -c