CC = g++

all: match.o packet.o server.o main.o 
	$(CC) match.o packet.o server.o main.o -o server -pthread

match.o: match.cpp
	$(CC) match.cpp -c

packet.o: packet.cpp
	$(CC) packet.cpp -c

server.o: server.cpp 
	$(CC) server.cpp -c

main.o: main.cpp
	$(CC) main.cpp -c