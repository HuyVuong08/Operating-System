CFLAGS=-Wall
DEFS = -DUSE_SOME_DEF
all: readline

readline: main.o readline.o
	gcc main.o readline.o -o readline $(DEFS)

readline.o: readline.c readline.h
	gcc -c readline.c $(CFLAGS) $(DEFS)

main.o: readline.h
	gcc -c main.c $(CFLAGS) $(DEFS)

clean:
	rm *.o readline