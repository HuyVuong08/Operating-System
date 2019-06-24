#include <iostream>
#include <unistd.h>

#include "message.hpp"

using namespace std;
struct Message * msg = NULL;

int main(){
    msg = (struct Message *)initSharedMemory();
    msg->bursttime = 0;
    msg->isMessageAvailable = 0;

    while (msg->isMessageAvailable != 1)
        sleep(1);

    printf("burst time = %d\n", msg->bursttime);
    return 0;
}