#ifndef SCHEDULERS_
#define SCHEDULERS_

#include <iostream>

using namespace std;

struct timeslot
{
    int starttime; /* Timestamp at the start of execution */
    int endtime;    /* Timestamp at the    end of execution */
};

struct process
{
    bool isCheck = false;
    
    /* Values initialized for each process */
    int arrivaltime;    // Timestamp process arrives and wish to start
    int bursttime;      // An amount of time process requires to complete the job
    /* Values algorithm may use to track process */
    struct timeslot* assigned_timeslot;
    int timeslot_count = 0;
    int timeslot_max;
    int flag;
};

class Scheduler{
private:
    int totalTime = 0, size;
    struct process ** list;
    struct process ** arrangedList;
    
    void totalTimeCalculate(struct process *pl, int pl_size){
        for (int i = 0; i < pl_size; i++)
            totalTime += pl[i].bursttime;
    }

    /*
     *  ShellSort: to ensure that the list is in ascending arrival time order
     */

    void sort(struct process ** &p, int n){
        for (int gap = n/2; gap > 0; gap /=2){
            for (int i = gap; i < n; i += 1) 
            { 
                // add a[i] to the elements that have been gap sorted 
                // save a[i] in temp and make a hole at position i 
                struct process * temp = p[i]; 
    
                // shift earlier gap-sorted elements up until the correct  
                // location for a[i] is found 
                int j;
                for (j = i; j >= gap && p[j - gap]->arrivaltime > temp->arrivaltime; j -= gap) 
                    p[j] = p[j - gap]; 
                
                //  put temp (the original a[i]) in its correct location 
                p[j] = temp; 
            } 
        }
    }
public:
    Scheduler(struct process *p, int pl_size): totalTime(0), size(pl_size), list(NULL), arrangedList(NULL){
        list = new struct process* [pl_size];
        arrangedList = new struct process* [pl_size];
        for (int i = 0; i < pl_size; i++)
            list[i] = &p[i];

        totalTimeCalculate(p, pl_size);
        sort(list, pl_size);
    }
    ~Scheduler(){
        delete[] list; delete list;
        totalTime = 0;
    }

    /* 
     * Scheduling algorithms
     */
    void FCFS();
    void SJF ();
    void SRTF();
    void RR  (int quantum_time);

    /*
     * Traversal
     */
    void traverse();
};


#endif
