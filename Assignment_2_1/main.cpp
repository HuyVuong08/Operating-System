
#include <iostream>
#include <unistd.h>

#include "schedulers.hpp"
#include "message.hpp"

#define MAX_PROCESS 50

struct Message * msg = NULL;

struct SharedMemory shrdMem;

int sched_dummy(struct process *pl, int pl_size)
{
   int i,j;
   for(i = 0;i < pl_size; i++){
      for(j = 0; j < 2; j++){
         pl[i].assigned_timeslot[j].starttime = pl[i].assigned_timeslot[j].endtime = 0;
         pl[i].timeslot_max = j+1;
      }
   }
   return 0;
}

/*
 * Main routine
 */

int main(int argc, char* argv[])
{
    // init for shared memory procedures
    msg = (struct Message *)shrdMem.initSharedMemory();
    msg->bursttime = 0;
    msg->isMessageAvailable = 0;


   char *ivalue = NULL;
   char *ovalue = NULL;
   char *qvalue = NULL;
   FILE *ifp, *ofp;
   struct process pl[MAX_PROCESS];
   int pl_size=0;
   int c,l;
   int quantum_time=1;
   char* endptr;
   int i;

   /* Program argument parsing */
   while ((c = getopt (argc, argv, "i:o:p:")) != -1)
      switch (c)
         {
            case 'i':
               ivalue = optarg;
               break;
            case 'o':
               ovalue = optarg;
               break;
            case 'p':
               qvalue = optarg;
               break;
         }

   /* Validate input arguments */
   if (ivalue == NULL){
      perror ("WARNING: use default input file");
      ivalue = (char*)"input.txt";
   }

   if (ovalue == NULL){
      perror ("WARNING: use default output file");
      ovalue = (char*)"output.txt";
   }

   if (qvalue != NULL){
      quantum_time = (int) strtol(qvalue, &endptr, 10); //base decimal
      if ( quantum_time < 1 || *endptr) {
          fprintf(stderr, "ERROR: Invalid quantum time\n");
          return (-1);
      }
      fprintf(stderr, "INFO: use passing argument quantum_time %d\n", quantum_time);
   }

   /* Get data input */
   ifp = fopen(ivalue, "r");
   if(ifp == NULL){
      perror ("ERROR: input file open failed\n");
      return (-1);
   }

   l = 0;
   while(fscanf(ifp,"%d %d",&pl[l].arrivaltime,&pl[l].bursttime) != EOF){
      /* Initialize the instance of struct process before using */
      pl[l].assigned_timeslot = (timeslot* )malloc(sizeof(struct timeslot) * 2); // assume the maximum number of time_slot is 10
      pl[l].timeslot_max = 0;
      pl[l].burtbackup = pl[l].bursttime;

      /* ACKnowledge a new process has been imported */
      pl_size = ++l;
   }

   /* Implement your scheduler */
   sched_dummy(pl, pl_size);

   l=0;
   ofp = fopen(ovalue, "w");
   if(ofp == NULL){
      perror ("ERROR: output file open failed\n");
      return (-1);
   }   



    
    Scheduler sched = Scheduler(pl, pl_size);
    //sched.FCFSs(msg);
    //sched.SJFs(msg);
    sched.SRTFs(msg);
    //sched.RR(msg, quantum_time);
    sched.traverse();

   /* Produce output data */
   while (l < pl_size)
   {
      for(i = 0; i < pl[l].timeslot_count; i++)
         fprintf(ofp, "Process %2d start %5d end%5d\n", l, 
                  pl[l].assigned_timeslot[i].starttime, pl[l].assigned_timeslot[i].endtime);
      l++;
   }


    

   return 0;
}
