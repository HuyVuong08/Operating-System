all: thread_debug

thread_debug: thread.o
	gcc -g thread.o -std=c11 -Wall -o thread
thread.o: thread.c thread.h
	gcc -g -c thread.c -std=c11 -Wall

clean:
	rm *o thread
