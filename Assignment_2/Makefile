all: sched

sched: main.o schedulers.o
	g++ -std=c++11 main.o schedulers.o -o sched 

main.o: main.cpp schedulers.hpp
	g++ -g -c -std=c++11 main.cpp 

schedulers.o: schedulers.cpp schedulers.hpp
	g++ -g -c -std=c++11 schedulers.cpp
clean:
	rm *.o sched
