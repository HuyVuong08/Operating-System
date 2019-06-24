#include    "ex1.h"
#include    <stdint.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <math.h>
#include    <time.h>

int main(){
    time_t t;
    srand(t);

    void *p[30];

    for (int i = 0; i < 30; i++){
        int tmp = (int)(pow(2, rand()% 15 ));
        p[i] = aligned_malloc(i * 93, tmp);
        if ((uintptr_t)(p[i]) % tmp == 0)
            printf("yay mee\n");
        else
            printf("Not fun\n");
        aligned_free(p[i]);
    }
    return 0;
}
