all: yayme

yayme: main.o ex1.o
	gcc -lm ex1.o main.o -o yayme

main.o: main.c ex1.h
	gcc -c main.c
ex1.o: ex1.c ex1.h
	gcc -c ex1.c

clean:
	rm *.o yayme