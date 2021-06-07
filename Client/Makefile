CC = g++
CFLAGS = -g -Wall -Werror -W -Wextra

all: match.o packet.o client.o
	$(CC) $(CFLAGS) match.o packet.o client.o -o client

match.o: match.cpp
	$(CC) $(CFLAGS) match.cpp -c

packet.o: ../packet.cpp
	$(CC) $(CFLAGS) ../packet.cpp -c

client.o: client.cpp
	$(CC) $(CFLAGS) client.cpp -c