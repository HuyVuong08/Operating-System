#include <iostream>

#include "message.hpp"

using namespace std;

struct Message * msg = NULL;
struct SharedMemory shrdMem;
/*
 * Params: int bursttime
 */
int main(int argn, char ** argc){
    if (argn < 2){
        cout << "usage: ./sender bursttime\n";
        return -1;
    }

    msg = (struct Message *)shrdMem.initSharedMemory();
    msg->isMessageAvailable = 1;
    msg->bursttime = atoi(argc[1]);

    return 0;

}