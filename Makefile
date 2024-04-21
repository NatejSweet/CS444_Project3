CC = gcc
CFLAGS = -Wall -Wextra

all: pc

main: pc.c
	$(CC) $(CFLAGS) -o pc pc.c

clean:
	rm -f pc