#include    <stdio.h>
#include    <pthread.h>
#include    <stdlib.h>
#include    <time.h>

#define CIRCLER     1

#define MAX_THREAD  5

unsigned int totalin = 0;
unsigned int total = 0;
unsigned int  p[MAX_THREAD];         // thread ith saves result at cell i

pthread_t   thrd_id[MAX_THREAD];

struct thread_data {
    int pos;
    int num;
};


struct thread_data  thrdDATA[MAX_THREAD];

double randCordinate(){
    return (double)rand()/RAND_MAX * 2 - 1;
}

short inCircle(double x, double y){
    
    if (x*x + y*y <= CIRCLER)
        return 1;
    else return 0;
}

void *threadroutine(void * stru){
    struct thread_data *s = (struct thread_data *)stru;
    for (int i = 0; i < s->num; i++){
        double x = randCordinate();
        double y = randCordinate();        
        if (inCircle(x, y) == 1)
            p[s->pos]++;
    }
    
    pthread_exit(NULL);
}
int main(int argc, char * args[]){
    clock_t begin = clock();
    if (argc != 2){
        fprintf(stderr, "usage: ./main  <number of points>\n");
        return -1;
    }

    if ((total = atoi(args[1])) < 0){
        fprintf(stderr, "number must be positive\n");
        return -1;
    }

    unsigned int point_th = total / MAX_THREAD;
    srand(time(NULL));
    for (int i = 0; i < MAX_THREAD; i++){
        thrdDATA[i].pos = i;
        thrdDATA[i].num = point_th;
        pthread_create(&thrd_id[i], NULL, threadroutine, (void *)&thrdDATA[i]);
        pthread_join(thrd_id[i], NULL);
        
    }

    for (int i = 0; i < MAX_THREAD; i++)
        totalin += p[i];
    printf("value of pi = %0.12f\n", 4 * (double)totalin / total);
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f\n", time_spent);
    return 0;
}
