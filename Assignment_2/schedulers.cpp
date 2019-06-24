#include    "schedulers.hpp"
#include    "DataStructure.hpp"

#include <string.h>

void Scheduler::FCFS(){
    int totalTimeRemaining = this->totalTime;
    int i = 0;
    unsigned int j = 0;
    for (; totalTimeRemaining != 0; j++){
        if (this->list[j]->arrivaltime > i)
            i = this->list[j]->arrivaltime;

        this->list[j]->timeslot_count = 1;
        this->list[j]->assigned_timeslot[0].starttime = i;
        i += this->list[j]->bursttime;
        this->list[j]->assigned_timeslot[0].endtime = i;
        
        this->arrangedList[j] = this->list[j];

        totalTimeRemaining -= this->list[j]->bursttime;
    }
}


void Scheduler::SJF(){
    int current = 0;        // control time
    int j = 0;              // maintain position of this->arrangedList
    int totalTimeRemaining = this->totalTime;
    struct process * min = NULL;

    for ( ;totalTimeRemaining != 0;){
        // find min bursttime process from 0 to current

        min = this->list[0];
        for (int i = 0; i < this->size; i++){
            if (!min->isCheck){
                if (this->list[i]->arrivaltime <= current)
                    if (min->bursttime > this->list[i]->bursttime && this->list[i]->isCheck == false)
                        min = this->list[i];
            }
            else
                min = this->list[i];
        }   

        min->isCheck = true;
        min->timeslot_count = 1;
        min->assigned_timeslot[0].starttime = current;
        current += min->bursttime;
        min->assigned_timeslot[0].endtime = current;        
        this->arrangedList[j] =  min;        
        totalTimeRemaining -= this->arrangedList[j]->bursttime;
        j++;
    }
}


void Scheduler::SRTF(){
    arrangedList = list;
   /*
    *  Pseudo code:
    *  while (totalRemainingTime > 0){    
    *    if (no_remaining_process)
    *       head = stack.getHead
    *       while (stack_is_not_empty)           
    *           add_to_timeslot(head.data)
    *           head = head->next
    *       exit_from_loop
    * 
    *    head_old = stack.getHead()
    *    stack.addToStack(next_element_in_array)
    *    head_new = stack.getHead()
    *    if (head_old != head_new)
    *       if (head_new arrives after head_old)
    *          totalRemainingTime = head_new.arrivalTime - head_old.arrivalTime
    *          head_old.
    *          update_slot_for(head_old)
    *       else //head_new and head_old same arrival time 
    *          do_nothing
    *  }
    */

    int totalTimeRemaining = this->totalTime;    
    SpecialStack stack;

    // iterator of list[]
    int i = 0;
    // time indicator
    int time = -1;
    Node *head = NULL, *head_prv = NULL;

    
    while (totalTimeRemaining > 0){
        head = stack.getHead();
        
        if (head != NULL){
            totalTimeRemaining--;
            if (head != head_prv){
                head->start(time);
                if (head_prv != NULL)
                    head_prv->suspend(time);
            }

            head_prv = head;
            if (--head->_data->bursttime == 0){
                head->suspend(time+1);
                stack.rmvHead();
                head_prv = NULL;
            }
        }
        time++;
        // if not reach end of arrangedList
        if (i != this->size){
            if (time < arrangedList[i]->arrivaltime){
                //do_nothing
            }
            else {
                do{
                    stack.push(arrangedList[i]);
                    i++;
                }
                while (i < this->size && arrangedList[i]->arrivaltime == arrangedList[i-1]->arrivaltime);
            }
        }
    }


    for (int i = 0; i < this->size; i++)
        arrangedList[i]->bursttime = list[i]->bursttime;
}

void Scheduler::RR(int quantum_time){
/*
 *  The issue of this algorithm is that each transition from old quantumn to new quantumn occurs
 *  at the same moment: 
 *      if quantumn = 1, at time = 1 there are 2 events occuring: old process ends up its quantumn time 
 *      and the new one replaces.
 */
    arrangedList = list;

    Queue queue;
    int totalTimeRemaining = this->totalTime;
    int time = -1;       // control time
    int i = 0;           // iterator of arrangedList[]
    int quantum = quantum_time;
    Node * head = NULL;
    

    while (totalTimeRemaining > 0){
        time++;

        //********* Preparation
        if (i < size && time == arrangedList[i]->arrivaltime){
            do{
                queue.enqueue(arrangedList[i++]);
            } while (i < size && arrangedList[i]->arrivaltime == arrangedList[i-1]->arrivaltime);
        }

        if (queue.isEmpty() == false && head == NULL){
            head = queue.dequeue();
            head->start(time);
        }
        //**********************


        if (head != NULL){
            quantum--;
            head->_data->bursttime--;
            totalTimeRemaining--;

            if ( quantum == 0 || head->_data->bursttime == 0){
                head->suspend(time+1);
                quantum = quantum_time;
                
                if (head->_data->bursttime == 0)
                    delete head;
                else
                    queue.enqueue(head);
                
                head = queue.dequeue();
                if (head != NULL)
                    head->start(time+1);
            }

        }
    }
   
    for (int i = 0; i < this->size; i++)
        arrangedList[i]->bursttime = list[i]->bursttime;
}
//////////////////////////////////////
void Scheduler::traverse()
{
    for (int i = 0; i < this->size; i++){
        cout << "- Process " << i << ": \n"
             << "     arrivalTime = " << this->arrangedList[i]->arrivaltime << "\n"
             << "     burstTime   = " << this->arrangedList[i]->bursttime << "\n";
             for (int j = 0; j < this->arrangedList[i]->timeslot_count; j++){
                cout
                    << "     timeSlot " << j << " : \n"
                    << "          + start: " << this->arrangedList[i]->assigned_timeslot[j].starttime << "\n"
                    << "          + end  : " << this->arrangedList[i]->assigned_timeslot[j].endtime << "\n";
            }
    }
}