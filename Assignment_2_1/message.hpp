#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAXSIZE     10

struct Message {
    bool isMessageAvailable = false;
    int bursttime = 0;
};

struct SharedMemory{
    int shmid;
    key_t key;
    void *shm;


void* initSharedMemory(){
    key = 1234;

    shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666);
    shm = shmat(shmid, NULL, 0);

    return shm;    
}
};


#endif