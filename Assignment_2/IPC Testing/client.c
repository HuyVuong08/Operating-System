#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "str.h"

#define MAXSIZE     15

void die(char *s)
{
    perror(s);
    exit(1);
}

int main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    key = 1234;

    if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
        die("shmat");

    struct Node * p = (struct Node *)shm;
    printf("p: data = %4d\n", p->_data);
    p->_next = (struct Node *)malloc(sizeof(struct Node));
    p->_next->_data = 13432;
    printf("address of next: %p\n", &(p->_next->_data));
    exit(0);
}
