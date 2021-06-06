CC = g++

all: match.o packet.o server.o main.o 
	$(CC) match.o packet.o server.o main.o -o server -pthread -g

match.o: match.cpp
	$(CC) match.cpp -c -g

packet.o: packet.cpp
	$(CC) packet.cpp -c -g

server.o: server.cpp 
	$(CC) server.cpp -c -g

main.o: main.cpp
	$(CC) main.cpp -c -g