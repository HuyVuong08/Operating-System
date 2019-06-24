all: sched sender

sched: main.o schedulers.o
	g++ -std=c++11 main.o schedulers.o -o sched 

main.o: main.cpp schedulers.hpp message.hpp
	g++ -g -c -std=c++11 main.cpp 

schedulers.o: schedulers.cpp schedulers.hpp message.hpp process.hpp
	g++ -g -c -std=c++11 schedulers.cpp

sender: sender.o
	g++ -std=c++11 sender.o -o sender
sender.o: sender.cpp message.hpp
	g++ -g -c -std=c++11 sender.cpp
clean:
	rm *.o sched sender
