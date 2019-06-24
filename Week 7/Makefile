all: week7

week7: main.o mem.o
	gcc -o week7 main.o mem.o -lpthread
main.o: main.c mem.h
	gcc -c -lpthread main.c 
mem.o: mem.o mem.h
	gcc -c -lpthread mem.c 

clean:
	rm *.o week7