all: rr fcfs sjf srtf
o: rr.o fcfs.o sjf.o srtf.o
fcfs.o: 
	@gcc -c fcfs.c
sjf.o:  
	@gcc -c sjf.c
srtf.o:
	@gcc -c srtf.c
rr.o:
	@gcc -c rr.c
fcfs: 
	@gcc -o fcfs fcfs.c
sjf:  
	@gcc -o sjf sjf.c
srtf: 
	@gcc -o srtf srtf.c
rr: 
	@gcc -o rr rr.c
clean :
	@rm -f *.o rr fcfs sjf srtf