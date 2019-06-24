#include    <stdio.h>
#include    <pthread.h>
#include    <string.h>

short order[10];

void * hello(void * tid) {
    int thrid = *((int *)tid);
    if (thrid != 0)
        while (order[thrid - 1] != 1);
    printf("Hello from thread %d\n", thrid);
    order[thrid] = 1;

    pthread_exit(NULL);
}

int main() {
    pthread_t tid[10];
    memset(order, 10, sizeof(short));

    int i;
    for (i = 0; i < 10; i++) {
        pthread_create(&tid[i], NULL, hello, (void*)&i);
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}
