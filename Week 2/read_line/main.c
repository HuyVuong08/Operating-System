#include    "readline.h"
#include    <stdio.h>

int main(){
    int n = 0 ;
    char buff[MAX_BUFF];
    do{
        n = read_line(buff);
        printf("ret value = %d and string %s\n", n, buff);        
    } while (n > 0);
    return 0;
}