CC=CC
CFLAGS=-O3
EXT=c

.SUFFIXES: .$(EXT)
#all: 1 

1: 1.thread_level_support

1.thread_level_support.o: 1.thread_level_support.$(EXT)
	$(CC) $(CFLAGS) -c 1.thread_level_support.$(EXT) -o 1.thread_level_support.o

clean:
	rm -f *.o
	rm -f 1.thread_level_support
