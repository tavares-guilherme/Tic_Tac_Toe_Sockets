CC = g++

all: match.o packet.o client.o
	$(CC) match.o packet.o client.o -o client

match.o: match.cpp
	$(CC) match.cpp -c

packet.o: packet.cpp
	$(CC) packet.cpp -c

client.o: client.cpp
	$(CC) client.cpp -c