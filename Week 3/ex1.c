#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define MAX_BUFF    4096

pid_t child;
char  buff[MAX_BUFF];

struct LinkedList{
    int                _num;
    struct LinkedList *_pNext;
} ;
int main(){
    size_t n = 0;
    struct LinkedList *p = NULL, *head = NULL;

    FILE * fp = fopen("numbers.txt", "rb");
    while (fgets(buff, MAX_BUFF, fp) != NULL){
        if (p == NULL){
            p = malloc(sizeof(struct LinkedList));
            head = p;
        }
        else{
            p->_pNext = malloc(sizeof(struct LinkedList));
            p = p->_pNext;
        }
        buff[strlen(buff) - 1] = 0;
        p->_num = atoi(buff);        
    }
    p = head;
    int num = 0;
    switch (fork()){
        case 0:{
            while (p){
                if (p->_num % 3 == 0)
                    num++;
                p = p->_pNext;
            }
            printf("%d\n", num);
            break;
        }
        default:{
            while (p){
                if (p->_num % 2 == 0)
                    num++;
                p = p->_pNext;
            }
            printf("%d\n", num);
        }
    }
    fclose(fp);
    return 0;
}