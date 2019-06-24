#include    <stdio.h>
#include    <pthread.h>
#include    <stdlib.h>
#include    <time.h>

#define CIRCLER     1

#define MAX_THREAD  5

unsigned int totalin = 0;
unsigned int total = 0;

static volatile unsigned int counter = 0;
pthread_mutex_t lock;

unsigned int point_th;





pthread_t   thrd_id[MAX_THREAD];



int thrdDATA[MAX_THREAD];

double randCordinate(){
    return (double)rand()/RAND_MAX * 2 - 1;
}

short inCircle(double x, double y){
    
    if (x*x + y*y <= CIRCLER)
        return 1;
    else return 0;
}

void *threadroutine(void * stru){
    unsigned int c = 0;
    for (int i = 0; i < point_th; i++){
        double x = randCordinate();
        double y = randCordinate();        
        if (inCircle(x, y) == 1){
            c++;
        }
    }

    pthread_mutex_lock(&lock);
    counter = counter + c;
    pthread_mutex_unlock(&lock);
    
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

    point_th = total / MAX_THREAD;
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < MAX_THREAD; i++){
        pthread_create(&thrd_id[i], NULL, threadroutine, (void *)&thrdDATA[i]);
        pthread_join(thrd_id[i], NULL);
        
    }

    totalin = counter;
    printf("value of pi = %0.12f\n", 4 * (double)totalin / total);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f\n", time_spent);

    return 0;
}
