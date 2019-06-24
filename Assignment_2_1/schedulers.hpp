#ifndef SCHEDULERS_
#define SCHEDULERS_

#include <iostream>

#include    "message.hpp"
#include    "process.hpp"
#include    "DataStructure.hpp"

using namespace std;


class Scheduler{
private:
    int totalTime = 0, size;
    struct process ** list;
    struct process ** arrangedList;

    Queue queueList = Queue();
    
    void totalTimeCalculate(struct process *pl, int pl_size){
        for (int i = 0; i < pl_size; i++)
            totalTime += pl[i].bursttime;
    }

    void msgProcessing(struct Message * msg, int time, int &totalTimeRemaining, SpecialStack *s);
    
public:
    Scheduler(struct process *p, int pl_size): totalTime(0), size(pl_size), list(NULL), arrangedList(NULL){
        list = new struct process* [pl_size];
        arrangedList = new struct process* [pl_size];
        for (int i = 0; i < pl_size; i++)
            queueList.enqueueAscending(&p[i]);
        

        totalTimeCalculate(p, pl_size);        
    }
    ~Scheduler(){
        delete[] list; delete list;
        totalTime = 0;
    }

    /* 
     * Scheduling algorithms
     */
    void FCFS(struct Message * msg);
    void SJF (struct Message * msg);
    void SRTF(struct Message * msg);

    void FCFSs(struct Message * msg);
    void SJFs (struct Message * msg);
    void SRTFs(struct Message * msg);

    void RR  (struct Message * msg, int quantum_time);

    /*
     * Traversal
     */
    void traverse();
};


#endif
