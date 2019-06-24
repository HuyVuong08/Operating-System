#include    "ex1.h"

#include    <stdio.h>
#include    <stdint.h>
#include    <stdlib.h>

void *aligned_malloc(unsigned int size, unsigned int align){
    void *mem = malloc(size + sizeof(void*) + (align - 1));
    
    void *ptr = (void**)((uintptr_t) (mem + (align - 1) + sizeof(void*)) & ~(align - 1));
    

    ((void **) ptr)[-1] = mem;    

    return ptr;
}

void *aligned_free(void *ptr){
    free(((void**) ptr)[-1]);
    return NULL;
}