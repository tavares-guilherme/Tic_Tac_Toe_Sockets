CC = g++
CFLAGS = -Wall -Wextra -pthread

all: match.o packet.o server.o main.o 
	$(CC) $(CFLAGS) match.o packet.o server.o main.o -o server 

match.o: match.cpp
	$(CC) $(CFLAGS) match.cpp -c

packet.o: packet.cpp
	$(CC) $(CFLAGS) packet.cpp -c

server.o: server.cpp 
	$(CC) $(CFLAGS) -g server.cpp -c

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c