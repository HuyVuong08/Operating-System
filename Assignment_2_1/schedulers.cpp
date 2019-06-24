#include    "schedulers.hpp"
#include    "DataStructure.hpp"
#include    "message.hpp"
#include    "process.hpp"

#include <string.h>
#include <unistd.h>
#include <time.h>

void delay1s(){
    clock_t t; 
    t = clock();
    while ( ((double)(clock() - t))/CLOCKS_PER_SEC < 1); 
}

void Scheduler::msgProcessing(struct Message * msg, int time, int &totalTimeRemaining, SpecialStack *s){
    if (msg->isMessageAvailable == 1){
        msg->isMessageAvailable = 0;
        struct process * tmp = new struct process;
        tmp->arrivaltime = time;
        tmp->burtbackup = tmp->bursttime = msg->bursttime;
        tmp->timeslot_max = 2;
        tmp->assigned_timeslot = (timeslot* )malloc(sizeof(struct timeslot) * 2);
        totalTimeRemaining += tmp->burtbackup;
        queueList.enqueueAscending(tmp);

        if (s != NULL)
            s->push(tmp);
        queueList.traverse();
    }
}

/*************************************************************************************/
void Scheduler::FCFSs(struct Message * msg){
    int totalTimeRemaining = this->totalTime;
    int i = 0;
    Node * pIterator = queueList._head;
    for (int i = 0; totalTimeRemaining > 0; i++){
        printf("Start second: %3d\n", i);
        delay1s();
        msgProcessing(msg, i, totalTimeRemaining, NULL);

        if (i >= pIterator->_data->arrivaltime){
            if (pIterator->_data->isCheck == false){
                pIterator->_data->isCheck = true;
                pIterator->start(i);
            }
            totalTimeRemaining--;
            if (--pIterator->_data->bursttime > 0){
                //pIterator->_data->bursttime--;
            }
            else {
                pIterator->suspend(i+1);            
                pIterator = pIterator->_next;
                if (pIterator == NULL)
                    break;
            }
        }
    }
}

void Scheduler::SJFs(struct Message * msg){  
    int totalTimeRemaining = this->totalTime;
    Node * min = queueList.findMinBurst(0);

    for (int current = 0; totalTimeRemaining > 0; current++){
        printf("Start second: %3d\n", current);
        delay1s();
        msgProcessing(msg, current, totalTimeRemaining, NULL);

        if (current >= min->_data->arrivaltime){
            if (min->_data->isCheck == false){
                min->_data->isCheck = true;
                min->start(current);
            }
            totalTimeRemaining--;
            if (--min->_data->bursttime == 0){
                min->suspend(current+1);
                do {
                    min = queueList.findMinBurst(current);
                    if (min == NULL){
                        if (totalTimeRemaining > 0)
                            current++;
                        else
                            return;
                    }
                    else break;
                } while (true);
            }        
        }
        
    }
}




void Scheduler::SRTF(struct Message * msg){
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
}

void Scheduler::SRTFs(struct Message * msg){
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
    Node * pIterator = queueList._head;
    // time indicator
    int time = -1;
    Node *head = NULL, *head_prv = NULL;

    
    while (totalTimeRemaining > 0){
        if (time >= 0){
            printf("Start second: %3d\n", time);
            delay1s();
            msgProcessing(msg, time, totalTimeRemaining, &stack);
        }
            


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
        // if (pIterator != NULL){
        //     if (time >= pIterator->_data->arrivaltime){
        //         do {
                    
        //             if (!(pIterator->_next != NULL && 
        //                 pIterator->_next->_data->arrivaltime == pIterator->_data->arrivaltime))      
        //                 break;
        //             pIterator = pIterator->_next;
        //         }
        //         while (true);
        //     }
        // }

        if (pIterator != NULL && time == pIterator->_data->arrivaltime){
            do {
                stack.push(pIterator->_data);
                if (pIterator!= NULL && pIterator->_next != NULL){
                    if (pIterator->_data->arrivaltime != pIterator->_next->_data->arrivaltime){
                        
                        pIterator = pIterator->_next;
                        break;
                    }
                }
                else
                    break;
                pIterator = pIterator->_next;

            } while (true);
        }

    }
}

void Scheduler::RR(struct Message * msg, int quantum_time){
/*
 *  The issue of this algorithm is that each transition from old quantumn to new quantumn occurs
 *  at the same moment: 
 *      if quantumn = 1, at time = 1 there are 2 events occuring: old process ends up its quantumn time 
 *      and the new one replaces.
 */

    Queue queue;
    int totalTimeRemaining = this->totalTime;
    int time = -1;       // control time
    int i = 0;           // iterator of arrangedList[]
    int quantum = quantum_time;
    Node * head = NULL;
    Node * prevhead = NULL;
    
    Node * pIterator = queueList._head;
    

    while (totalTimeRemaining > 0){
        time++;
        printf("Start second: %3d, remainingTime = %4d\n", time, totalTimeRemaining);
        //delay1s();
        msgProcessing(msg, time, totalTimeRemaining, NULL);
        //********* Preparation
        if (pIterator != NULL && time == pIterator->_data->arrivaltime){
            do {
                queue.enqueue(pIterator->_data);
                if (pIterator!= NULL && pIterator->_next != NULL){
                    if (pIterator->_data->arrivaltime != pIterator->_next->_data->arrivaltime){
                        
                        pIterator = pIterator->_next;
                        break;
                    }
                }
                else
                    break;
                pIterator = pIterator->_next;

            } while (true);
        }
        if (!queue.isEmpty() && time > queue._head->_data->arrivaltime){
            if (prevhead != NULL)
                queue.enqueue(prevhead);
            head = queue.dequeue();
            if (head != NULL)
                head->start(time);
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
                    prevhead = head;              
                
            }

        }
    }
}
//////////////////////////////////////
void Scheduler::traverse()
{
    int i = 0;
    for (Node * p = queueList._head;  p != NULL; p = p->_next, i++){
            cout << "- Process " << i << ": \n"
             << "     arrivalTime = " << p->_data->arrivaltime << "\n"
             << "     burstTime   = " << p->_data->burtbackup << "\n";
             for (int j = 0; j < p->_data->timeslot_count; j++){
                cout
                    << "     timeSlot " << j << " : \n"
                    << "          + start: " << p->_data->assigned_timeslot[j].starttime << "\n"
                    << "          + end  : " << p->_data->assigned_timeslot[j].endtime << "\n";
            }
    }    
}