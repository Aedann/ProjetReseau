CC = gcc
CFLAGS = -Wall -Wextra

all: client.out server.out

client.out: client/main.c client/client.c client/client.h
	$(CC) $(CFLAGS) -o $@ $^

server.out: server/main.c server/server.c server/bank.c server/server.h server/structBank.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f client.out server.out

