#include    <stdio.h>

struct Bamboo{
    char iss [13];
    int r;
}; 

int main(int argc, char const *argv[])
{
    printf("sizeof = %d", sizeof(struct Bamboo));
    return 0;
}
