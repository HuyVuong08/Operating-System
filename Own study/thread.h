#ifndef THREAD_H_
#define THREAD_H_

#define MAXLINE     4096

#include    <unistd.h>
#include    <sys/types.h>
#include    <stdio.h>
#include    <stdlib.h>


void    err_sys(const char *message){
    printf("%s", message);
    exit(-1);
}
#endif