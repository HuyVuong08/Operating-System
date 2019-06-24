#include    <unistd.h>
#include    <stdio.h>

// ******************* F1 generation ******************

void createBgenealogy();
void createCgenealogy();

int main(){     // this is A
    switch (fork()){
        case 0: {       // created B
            printf("pid in child=%d and parent=%d\n",getpid(),getppid());
            createBgenealogy();
            break;
        }
        default:{   // still in A
            switch (fork()){
                case 0:{        // created C
                    printf("pid in child=%d and parent=%d\n",getpid(),getppid());
                    createCgenealogy();
                    break;
                }
                default:{       // still in A
                    if (fork() == 0) // create D
                        printf("pid in child=%d and parent=%d\n",getpid(),getppid());
                    break;
                }
            }
        }
    }

    return 0;
}

void createBgenealogy(){
    switch (fork()){    // in B
        case 0:{                    // created E   
            printf("pid in child=%d and parent=%d\n",getpid(),getppid());
            if (fork() == 0)                 // created I
                printf("pid in child=%d and parent=%d\n",getpid(),getppid());
            break;
        }
        default:{       // still in B
            if (fork() == 0)       // created F
                printf("pid in child=%d and parent=%d\n",getpid(),getppid());
        }
    }
}
void createCgenealogy(){
    if (fork() == 0)         //created G
        printf("pid in child=%d and parent=%d\n",getpid(),getppid());   

}