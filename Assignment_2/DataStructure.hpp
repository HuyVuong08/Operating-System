/*
 * Tommy
 * October 27, 2018
 */

#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <string.h>

#include "schedulers.hpp"


class Node{
public:
    struct process * _data;
    Node           * _next;


    Node(struct process * data): _data(NULL), _next(NULL){
        _data = data;
    }
    ~Node(){
        _data = NULL;
        _next = NULL;
    }


    void start(int time){
        _data->assigned_timeslot[_data->timeslot_count].starttime = time;
    }
    void suspend(int time){
        _data->assigned_timeslot[_data->timeslot_count].endtime = time;
        if (++_data->timeslot_count == _data->timeslot_max){
            _data->timeslot_max *= 2;
            struct timeslot *tmp = new struct timeslot[_data->timeslot_max];
            memcpy(tmp, _data->assigned_timeslot, sizeof(struct timeslot)*(_data->timeslot_count));
            delete _data->assigned_timeslot;
            _data->assigned_timeslot = tmp;
        }
    }
};


class Queue{
private:
    Node * _head, * _tail;

public:
    Queue(struct process *p): _head(NULL), _tail(NULL){
        _head = _tail = new Node(p);
    }
    Queue(): _head(NULL), _tail(NULL){}
    ~Queue(){
        _head = _tail = NULL;
    }

    bool isEmpty(){
        return (_head == _tail && _head == NULL);
    }
    void enqueue(struct process * p){
        if (isEmpty())
            _head = _tail = new Node(p);
        else {
            _tail->_next = new Node(p);
            _tail = _tail->_next;
        }        
    }
    void enqueue(Node *p){
        if (isEmpty())
            _head = _tail = p;
        else {
            _tail->_next = p;
            _tail = p;
        }
    }
    Node* dequeue(){
        if (isEmpty())
            return NULL;
        
        Node * p = this->_head;
        if (_head == _tail)
            _head = _tail = NULL;
        else
            _head = _head->_next;

        return p;
    }
};

/* Tommy */
/*
 * Unlike normal stack which we push new item to the top of the stack, 
 * we push to a position that the ascending characteristic in unchanged
 */
class SpecialStack{
private:
    Node    *_head;

    bool isEmpty(){
        return _head == NULL;
    }
public:
    SpecialStack(): _head(NULL){}
   ~SpecialStack(){
        for (Node  * p = this->_head; _head != NULL;){
            _head = _head->_next;
            delete p;
            p = _head;
        }
    }

    // find the position to add Node(p) to stack
    void push(struct process *p){
        // stack empty, add to the top
        if (isEmpty()){
            _head = new Node(p);
            return;
        }
        
        // if stack is not empty
        Node * pNde = new Node(p);
        if (p->bursttime < _head->_data->bursttime){                // stack not empty but the position stands before _head
            pNde->_next = _head;
            _head = pNde;            
        } else {
            Node * tmp = _head;
            while (tmp->_next != NULL){
                // if the position is in the middle of stack
                if (tmp->_next->_data->bursttime > pNde->_data->bursttime){             
                    pNde->_next = tmp->_next;
                    tmp->_next = pNde;
                    return ;
                }
                else
                    tmp = tmp->_next;
            }
            // the position is at the bottom of stack
            tmp->_next = pNde;
            return;
        }
    }

    Node* getHead(){
        if (_head == NULL)
            return NULL;
        return _head;
    }

    void rmvHead(){
        Node * p = _head;
        _head = _head->_next;
        delete p;
    }


};
#endif