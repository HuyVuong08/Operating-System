#include    <stdio.h>

#define     MAX     5
int arrivlTime[] = {0, 2, 3, 5, 6};
int burstTime[]  = {10, 29, 3, 7, 12};

int minRmnTime(int bound){      // bound: number of living process
    int min = 0;
    for (short i = 1; i < bound; i++)
        if (burstTime[min] > burstTime[i])
            min = i;
    return min;
}

int isProcessRemain(){
    return (burstTime[0] + burstTime[1] + burstTime[2] + burstTime[3] + burstTime[4] != 0) ? 1 : 0;
}
int main(){
    int i = 0;
    while (isProcessRemain() > 0){
        int bound = 1;
        // check if process is loaded into ready queue
        for (int j = 1; j < MAX; j++){
            if (arrivlTime[j] > i)
                break;
            else
                bound++;
        }

        int conProcess = minRmnTime(bound);
        burstTime[conProcess]--;
        i++;        
    }

    printf("i = %d\n", i);
    return 0;
}