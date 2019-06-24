#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 50

/*
 * User defined structure
 */

struct timeslot
{
   int starttime; /* Timestamp at the start of execution */
   int endtime;   /* Timestamp at the   end of execution */
};

struct process
{
   /* Values initialized for each process */
   int arrivaltime;  /* Timestamp process arrives and
                        wish to start */
   int bursttime;    /* An amount of time process 
                        requires to complete the job */

   /* Values algorithm may use to track process */
   struct timeslot* assigned_timeslot;
   int timeslot_count;
   int flag;
};

/*
 * Implement scheduling algorithm
 */

int sched_dummy(struct process *pl, int pl_size)
{
   int i,j;
   for(i = 0;i < pl_size; i++){
      for(j = 0; j < 2; j++){
         pl[i].assigned_timeslot[j].starttime = pl[i].assigned_timeslot[j].endtime = 0;
         pl[i].timeslot_count = j+1;
      }
   }
   return 0;
}

/*
 * Main routine
 */

int main(int argc, char* argv[])
{
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
      ivalue = "input.txt";
   }

   if (ovalue == NULL){
      perror ("WARNING: use default output file");
      ovalue = "output.txt";
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
      pl[l].assigned_timeslot = malloc(sizeof(struct timeslot) * 10); // assume the maximum number of time_slot is 10
      pl[l].timeslot_count = 0;

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
