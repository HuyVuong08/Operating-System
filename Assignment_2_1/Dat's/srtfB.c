#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MAX_PROCESS 50
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int timecounter,qt;
int pl_size=0;
int finish=0;
void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
} 	
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
	int number;
	int flag;
	float ratio;

};
void swap_p (struct process *a, struct process*b)
{
	struct process temp=*a;
	*a=*b;
	*b=temp;
}
void sort_on_arrival_time(struct process *pl) {
	int i,j;
		for (i = 0; i < pl_size-1; i++) {
			int min=pl[i].arrivaltime;
			for (j=i+1;j<pl_size;j++)
			{
				if (pl[i].arrivaltime>pl[j].arrivaltime) {
					swap_p(&pl[i],&pl[j]);
				}
			}
		}
	
			
	}
 struct process pl[MAX_PROCESS];
void* sched_fcfs() //1
{
   int l = 0,i,k,j,prev=-1,c;	
   while (finish==0)
   {
      	sort_on_arrival_time(pl);
	while (timecounter>=pl[l].arrivaltime &l <pl_size) l++;
	for (i=0;i<l;i++)
	if (pl[i].bursttime>0) break;
	for (j=i+1;j<l;j++) // check the min bursttime
	{
		if (pl[j].bursttime>0 & pl[i].bursttime>pl[j].bursttime) i=j;
	}
	if (pl[i].bursttime>0)
	{
		if (prev!=i) // check if the process start new slot time -> set start time  
		{
			pl[i].timeslot_count=pl[i].timeslot_count+1;
			c=pl[i].timeslot_count-1;
			pl[i].assigned_timeslot[c].starttime=timecounter;
		}
		pthread_mutex_lock(&mutex);
		printf(" Time %2d-%2d Proccess %5d \n",timecounter,timecounter+1,pl[i].number);
		delay(500);
		timecounter++;
		pl[i].bursttime= pl[i].bursttime-1;
		pthread_mutex_unlock(&mutex);
		prev=i;
		pl[i].assigned_timeslot[c].endtime=timecounter;	
		if ( pl[i].bursttime==0) printf(" Time %5d Proccess %5d end\n",timecounter,pl[i].number );
	}
	else
	{
		timecounter++;
		printf(" Time %5d No proccess\n",timecounter-1);
	}
	finish=1; // check all the proccess is finish?
	for (i=0;i<pl_size;i++)
	if (pl[i].bursttime>0)  finish=0;
    }
   finish=1;
   return 0;
}
void* extend()
{
	while(finish==0)
	{
		int n;
		scanf("%d",&n);
		if ( n>0)
		{
			pthread_mutex_lock(&mutex);
			pl[pl_size].assigned_timeslot = malloc(sizeof(struct timeslot) * 10); // assume the maximum number of time_slot is 10
			pl[pl_size].timeslot_count = 0;
			pl[pl_size].number=pl_size;
			pl[pl_size].arrivaltime=timecounter;
			pl[pl_size].flag=pl[pl_size].bursttime=n;
			if (finish==0) printf("Insert Successfully\nProccess %d with burst time %d arrival at %d\n",pl[pl_size].number,pl[pl_size].bursttime,pl[pl_size].arrivaltime);
			else printf("Finish\n");
			pl_size++;
			pthread_mutex_unlock(&mutex);
		}
		else printf("Insert failed\n");
	}
}

/*
 * Main routine
 */

 
int main(int argc, char* argv[])
{
   pthread_t threads[2];
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   
   char *ivalue = NULL;
   char *ovalue = NULL;
   char *qvalue = NULL;
   FILE *ifp, *ofp;
  
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

	if (ifp == NULL) {
		perror("ERROR: input file open failed\n");
		return (-1);
	}

	l = 0;

	while (fscanf(ifp, "%d %d", &pl[l].arrivaltime, &pl[l].bursttime) != EOF) {
		/* Initialize the instance of struct process before using */
		pl[l].assigned_timeslot = malloc(sizeof(struct timeslot) * 10); // assume the maximum number of time_slot is 10
		pl[l].timeslot_count = 0;
		pl[l].number=l;
		pl[l].number=l;
		/* ACKnowledge a new process has been imported */
		pl[l].flag=pl[l].bursttime;
		pl_size = ++l;
		

	}

	/* Initialize system clock */
   timecounter = 0;
   //sched_fcfs(pl);
   pthread_create(&threads[0], &attr, sched_fcfs, (void *) NULL);
   pthread_create(&threads[1], &attr, extend, (void *) NULL);
   pthread_join(threads[0], NULL);
   pthread_join(threads[1], NULL);
   int k = 0;
   pthread_mutex_destroy(&mutex);

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
         printf( "Process %2d start %5d end%5d\n", pl[l].number, 
                  pl[l].assigned_timeslot[i].starttime, pl[l].assigned_timeslot[i].endtime);
      	l++;
   }
   float wt[MAX_PROCESS],tat[MAX_PROCESS],rp[MAX_PROCESS],avwt=0,avtat=0,avrp=0,throughput=0;
   int v;
   for (v=0;v<pl_size;v++)
   {
   		int c=pl[v].timeslot_count-1;   		
   		wt[v]=pl[v].assigned_timeslot[c].endtime - pl[v].arrivaltime -pl[v].flag;
   		rp[v]=pl[v].assigned_timeslot[0].starttime - pl[v].arrivaltime;
   		avwt+=wt[v];
   		avrp+=rp[v];
   }
   for (v=0;v<pl_size;v++)
   {
   		tat[v]=wt[v] + pl[v].flag;
   		avtat+=tat[v];
   }
   avrp/=pl_size;
   avwt/=pl_size;
   avtat/=pl_size;
   throughput+=pl_size;
   throughput/=timecounter;
   fprintf (ofp, "Throughput = %.2f\n", throughput);
   fprintf (ofp, "Average waiting time = %.2f ms\n", avwt);
   fprintf (ofp, "Average response time = %.2f ms\n", avrp);
   fprintf (ofp, "Average turnaround time = %.2f ms\n", avtat); 
	return 0;

}

