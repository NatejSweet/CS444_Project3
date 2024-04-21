CC = gcc
CFLAGS = -g -Wall

pc: pc.c eventbuf.c
	$(CC) $(CFLAGS) -o pc pc.c eventbuf.c

clean:
	rm -f pc
