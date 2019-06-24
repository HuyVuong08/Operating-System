#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 50

int timecounter;

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

int compare(const void *a, const void *b)
{
	int l = ((struct process *)a)->arrivaltime;
	int r = ((struct process *)b)->arrivaltime;
	return (l - r);
}

int sort_on_arrival_time(struct process *pl, int pl_size) {
	qsort((void*)pl, pl_size, sizeof(pl[0]), compare);
}


int sched_fcfs(struct process *pl, int pl_size)
{
   int l = 0;

   /* TODO: sort arrival time */
   sort_on_arrival_time(pl, pl_size);

   /* Do loop simulation engine */
   while (pl_size > 0)
   {
      /* Perform an event */
      if(timecounter >= pl[l].arrivaltime)
      {
         /* TODO: execute that process */
         pl[l].assigned_timeslot[0].starttime = timecounter;
         timecounter += pl[l].bursttime;
         pl[l].assigned_timeslot[0].endtime = timecounter;
         pl[l].timeslot_count = 1;

         /* Update a completed task */
         l++;
         pl_size--;
      }else{
         /* Set clock to next event time */
         timecounter++;
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
      ovalue = "/home/mercedes-a200/OS_Assignment/Output/output_FCFS.txt";
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

   /* Initialize system clock */
   timecounter = 0;

   /* Implement your scheduler */
// sched_dummy(pl, pl_size);
   sched_fcfs(pl, pl_size);

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

   /* TODO: calculate scheduling criteria 
            base on pl (=process_list) which is already existed */
   float throughput = 0, wait = 0, response = 0, turnaround = 0;
   throughput = (float)l / timecounter;
   for (i = 0; i < l; i++) {
	   if (pl[i].timeslot_count == 1) {
		   wait = wait + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
	   }
	   else {
           int j;
		   for (j = 0; j < pl[i].timeslot_count; j++) {
			   if (j == 0) {
				   wait = wait + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
			   }
			   else {
				   wait = wait + pl[i].assigned_timeslot[j].starttime - pl[i].assigned_timeslot[j - 1].endtime;
			   }
		   }
	   }
	   turnaround += pl[i].assigned_timeslot[pl[i].timeslot_count - 1].endtime - pl[i].arrivaltime;
	   response += pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
   }
   fprintf(ofp, "Throughput time: %.2g\n", throughput);
   fprintf(ofp, "Average waiting time: %.2g\n", wait/(float)l);
   fprintf(ofp, "Average response time: %.2g\n", response/(float)l);
   fprintf(ofp, "Average turnaround time: %.2g\n", turnaround/ (float)l);

   return 0;
}
