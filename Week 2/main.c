#include    "findsubstr.h"
#include    "readline.h"
#include    <stdio.h>
#include    <string.h>

char buff[MAX_BUFF];
int  n = 0;

int main(int argc, char **argv){
    if (argc != 2){
        printf("usage: mygrep <input_string>\n");
        return -1;
    }    

    do{
        n = read_line(buff);        
        if (find_sub_string(buff, argv[1]) >= 0)
            printf("%s", buff);
    } while (n > 0);

    return 0;
}