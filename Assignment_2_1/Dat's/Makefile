fcfs: fcfsB.o
	gcc -o fcfs fcfsB.o -lpthread
fcfsB.o: fcfsB.c
	gcc -c -lpthread fcfsB.c

sjf: sjfB.o
	gcc -o sjf sjfB.o -lpthread
sjfB.o: sjfB.c
	gcc -c -lpthread sjfB.c

srtf: srtfB.o
	gcc -o srtf srtfB.o -lpthread
srtfB.o: srtfB.c
	gcc -c -lpthread srtfB.c

rr: rrB.o
	gcc -o rr rrB.o -lpthread
rrB.o: rrB.c
	gcc -c -lpthread rrB.c 

clean:
	rm *.o rr srtf sjf fcfs