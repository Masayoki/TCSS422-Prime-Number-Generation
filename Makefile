CC=gcc
CFLAGS=-pthread -I. -Wall -Wno-int-conversion -D_GNU_SOURCE

binaries=prime

all: $(binaries)

prime: buffer.c prime.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) -f $(binaries) *.o


 
