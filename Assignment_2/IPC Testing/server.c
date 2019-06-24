//SHMServer.C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "str.h"

#define MAXSIZE     15

void die(char *s)
{
    perror(s);
    exit(1);
}

int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    key = 1234;

    if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
        die("shmat");

    /*
     *      * Put some things into the memory for the
     *        other process to read.
     *        */

    struct Node * p = (struct Node *)shm;
    p->_data = 100;
    p->_next = NULL;



    /*
     * Wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (p->_next == NULL)
        sleep(1);

    
    printf("address of next: %p\n", &(p->_next->_data));
    printf("value: %d", (p->_next->_data));
    exit(0);
}