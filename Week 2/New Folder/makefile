CFLAGS=-Wall
DEFS = -DUSE_SOME_DEF
all: mygrep

mygrep: main.o findsubstr.o readline.o
	gcc main.o findsubstr.o readline.o -o mygrep $(DEFS)

findsubstr.o: findsubstr.c findsubstr.h
	gcc -c findsubstr.c $(CFLAGS) $(DEFS)

readline.o: readline.c readline.h
	gcc -c readline.c $(CFLAGS) $(DEFS)

main.o: readline.h findsubstr.h
	gcc -c main.c $(CFLAGS) $(DEFS)

clean:
	rm *.o mygrep