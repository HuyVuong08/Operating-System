#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>

#define TIME_UNIT	100 // In microsecond
#define MAX_PROCESS 50
#define INT_MAX 2147483647

int timecounter;

/*
 * User defined structure
 */

struct timeslot
{
   int starttime; /* Timestamp at the start of execution */
   int endtime;   /* Timestamp at the   end of execution */
};

/* The PCB of a process */
struct pcb_t {
    /* Values initialized for each process */
    int arrival_time; 	// The timestamp at which process arrives
    // and wishes to start
    int burst_time;		// The amount of time that process requires
    // to complete its job
    int pid;		// process id
    struct timeslot* assigned_timeslot;
    int timeslot_count;
};

/* 'Wrapper' of PCB in a queue */
struct qitem_t {
    struct pcb_t * data;
    struct qitem_t * next;
};

/* The 'queue' used for both ready queue and in_queue (e.g. the list of
 * processes that will be loaded in the future) */
struct pqueue_t {
    /* HEAD and TAIL for queue */
    struct qitem_t * head;
    struct qitem_t * tail;
    /* MUTEX used to protect the queue from
     * being modified by multiple threads*/
    pthread_mutex_t lock;
};

void initialize_queue(struct pqueue_t * q) {
    q->head = q->tail = NULL;
    pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t * q) {
    return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */
struct pcb_t * de_queue(struct pqueue_t * q) {
    struct pcb_t * proc = NULL;
    // TODO: return q->head->data and remember to update the queue's head
    // and tail if necessary. Remember to use 'lock' to avoid race
    // condition

    // YOUR CODE HERE
    if(q->head == NULL)
        return proc;
    pthread_mutex_lock(&q->lock);
    proc = q->head->data;
    q->head = q->head->next;
    pthread_mutex_unlock(&q->lock);
    return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t * q, struct pcb_t * proc) {
    // TODO: Update q->tail.
    // Remember to use 'lock' to avoid race condition

    // YOUR CODE HERE
    struct qitem_t * ptr = (struct qitem_t *)malloc(sizeof(struct qitem_t));
    ptr->data = proc;
    ptr->next = NULL;
    pthread_mutex_lock(&q->lock);
    if(q->head == NULL)
    {
        q->tail = ptr;
        q->head = q->tail;
    }
    else
    {
        q->tail->next = ptr;
        q->tail = q->tail->next;
    }
    pthread_mutex_unlock(&q->lock);
}

static struct pqueue_t in_queue; // Queue for incomming processes
static struct pqueue_t ready_queue; // Queue for ready processes
static struct pqueue_t result_queue;

static int load_done = 0;
int timecounter;

static int timeslot = 2; 	// The maximum amount of time a process is allowed
// to be run on CPU before being swapped out

// Emulate the CPU
void * cpu(void * arg) {
    int timestamp = 0;
    /* Keep running until we have loaded all process from the input file
     * and there is no process in ready queue */
    while (!load_done || !empty(&ready_queue)) {
        /* Pickup the first process from the queue */
        struct pcb_t * proc = de_queue(&ready_queue);
        if (proc == NULL) {
            /* If there is no process in the queue then we
             * wait until the next time slice */
            timestamp++;
            usleep(TIME_UNIT);
        }else {
            /* Execute the process */
            if (timestamp < proc->arrival_time) {
                en_queue(&ready_queue, proc);
            }
            proc->assigned_timeslot[proc->timeslot_count].starttime = timestamp;    // Save timestamp
            int id = proc->pid;    // and PID for tracking
            /* Decide the amount of time that CPU will spend
             * on the process and write it to 'exec_time'.
             * It should not exeed 'timeslot'.
            */
            int exec_time = 0;

            // TODO: Calculate exec_time from process's PCB

            // YOUR CODE HERE
            if (proc->burst_time > timeslot) {
                exec_time += timeslot;
                proc->burst_time -= timeslot;
            } else {
                exec_time += proc->burst_time;
                proc->burst_time = 0;
            }
            /* Emulate the execution of the process by using
             * 'usleep()' function */
            usleep(exec_time * TIME_UNIT);

            /* Update the timestamp */
            timestamp += exec_time;
            proc->assigned_timeslot[proc->timeslot_count].endtime = timestamp;
            proc->timeslot_count++;

            // TODO: Check if the process has terminated (i.e. its
            // burst time is zero. If so, free its PCB. Otherwise,
            // put its PCB back to the queue.

            // YOUR CODE HERE
            if (proc->burst_time != 0) {
                proc->arrival_time = timestamp;
                en_queue(&ready_queue, proc);
            }
            else
            {
                en_queue(&result_queue, proc);
            }
            /* Track runtime status */
            timecounter = timestamp;
        }
    }
}

// Emulate the loader
void * loader(void * arg) {
    int timestamp = 0;
    /* Keep loading new process until the in_queue is empty*/
    while (!empty(&in_queue)) {
        struct pcb_t * proc = de_queue(&in_queue);
        /* Loader sleeps until the next process available */
        int wastetime = proc->arrival_time - timestamp;
        usleep(wastetime * TIME_UNIT);
        /* Update timestamp and put the new process to ready queue */
        timestamp += wastetime;
        en_queue(&ready_queue, proc);
    }
    /* We have no process to load */
    load_done = 1;
}

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

int sched_sjf(struct process *pl, int pl_size)
{
   int l = 0;
   int n = pl_size;
   int i;
   for(i = 0; i< pl_size; i ++)
      pl[i].flag = 0;

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
         pl[l].flag = 1;
         /* Update a completed task */
         pl_size--;
         int k = 0;
         for(i = 0; i < n; i ++)
         {
            if(pl[i].flag == 1)
               continue;
            else
            {
               if(i == 0)
               {
                  k = i;
                  continue;
               }
               if(pl[i].arrivaltime <= timecounter && pl[i].bursttime < pl[k].bursttime)
                  k = i;
            }
         }
         l = k;
      }else{
         /* Set clock to next event time */
         timecounter++;
      }
   }

   return 0;
}

int sched_srtf(struct process *pl, int pl_size)
{
   int l = 0;
   int bt[MAX_PROCESS];
   int i;
   int min = INT_MAX;
   int prepos, curpos = - 1;
   int temp;
   int check = 0;

   /* TODO: sort arrival time */
   sort_on_arrival_time(pl, pl_size);
   for (i = 0; i < pl_size; i++) {
	   bt[i] = pl[i].bursttime;
   }


   /* Do loop simulation engine */
   while (l < pl_size)
   {
	   /* Perform an event */
	   prepos = curpos;
	   for (i = 0; i < pl_size; i++) {
		   if ((pl[i].arrivaltime <= timecounter) && (bt[i] < min) && bt[i] > 0) {
			   min = bt[i];
			   curpos = i;
			   check = 1;
		   }
	   }
	   if (check == 0) {
		   timecounter++;
		   continue;
	   }
	   if (check == 1) {
		   if (prepos != curpos) {
			   pl[curpos].timeslot_count++;
			   pl[curpos].assigned_timeslot[pl[curpos].timeslot_count - 1].starttime = timecounter;
			   if (prepos != -1) pl[prepos].assigned_timeslot[pl[prepos].timeslot_count - 1].endtime = timecounter;
			   prepos = curpos;
		   }
		   bt[curpos]--;
		   min = bt[curpos];

		   if (min <= 0) min = INT_MAX;
		   if (bt[curpos] == 0) {
			   pl[curpos].assigned_timeslot[pl[curpos].timeslot_count - 1].endtime = timecounter + 1;
			   l++;
			   if (l == pl_size) {
				   if (prepos != -1) pl[prepos].assigned_timeslot[pl[prepos].timeslot_count - 1].endtime = timecounter + 1;
			   }
		   }
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
   float throughput[4] = {};
   float wait[4] = {};
   float response[4] = {};
   float turnaround[4] = {};
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
      ovalue = "/home/mercedes-a200/OS_Assignment/Output/output_E.txt";
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

   /* TODO: calculate scheduling criteria 
            base on pl (=process_list) which is already existed */
   throughput[0] = 0, wait[0] = 0, response[0] = 0, turnaround[0] = 0;
   throughput[0] = (float)l / timecounter;
   for (i = 0; i < l; i++) {
	   if (pl[i].timeslot_count == 1) {
		   wait[0] = wait[0] + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
	   }
	   else {
           int j;
		   for (j = 0; j < pl[i].timeslot_count; j++) {
			   if (j == 0) {
				   wait[0] = wait[0] + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
			   }
			   else {
				   wait[0] = wait[0] + pl[i].assigned_timeslot[j].starttime - pl[i].assigned_timeslot[j - 1].endtime;
			   }
		   }
	   }
	   turnaround[0] += pl[i].assigned_timeslot[pl[i].timeslot_count - 1].endtime - pl[i].arrivaltime;
	   response[0] += pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
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
   sched_sjf(pl, pl_size);

   l=0;
   ofp = fopen(ovalue, "w");
   if(ofp == NULL){
      perror ("ERROR: output file open failed\n");
      return (-1);
   }

   /* TODO: calculate scheduling criteria 
            base on pl (=process_list) which is already existed */
   throughput[1] = 0, wait[1] = 0, response[1] = 0, turnaround[1] = 0;
   throughput[1] = (float)l / timecounter;
   for (i = 0; i < l; i++) {
      if (pl[i].timeslot_count == 1) {
         wait[1] = wait[1] + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
      }
      else {
         int j;
         for (j = 0; j < pl[i].timeslot_count; j++) {
            if (j == 0) {
               wait[1] = wait[1] + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
            }
            else {
               wait[1] = wait[1] + pl[i].assigned_timeslot[j].starttime - pl[i].assigned_timeslot[j - 1].endtime;
            }
         }
      }
      turnaround[1] += pl[i].assigned_timeslot[pl[i].timeslot_count - 1].endtime - pl[i].arrivaltime;
      response[1] += pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
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
   sched_srtf(pl, pl_size);

   l=0;
   ofp = fopen(ovalue, "w");
   if(ofp == NULL){
      perror ("ERROR: output file open failed\n");
      return (-1);
   }

    /* TODO: calculate scheduling criteria
             base on pl (=process_list) which is already existed */
    throughput[2] = 0, wait[2] = 0, response[2] = 0, turnaround[2] = 0;
    throughput[2] = (float)l / timecounter;
    for (i = 0; i < l; i++) {
        if (pl[i].timeslot_count == 1) {
            wait[2] = wait[2] + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
        }
        else {
            int j;
            for (j = 0; j < pl[i].timeslot_count; j++) {
                if (j == 0) {
                    wait[2] = wait[2] + pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
                }
                else {
                    wait[2] = wait[2] + pl[i].assigned_timeslot[j].starttime - pl[i].assigned_timeslot[j - 1].endtime;
                }
            }
        }
        turnaround[2] += pl[i].assigned_timeslot[pl[i].timeslot_count - 1].endtime - pl[i].arrivaltime;
        response[2] += pl[i].assigned_timeslot[0].starttime - pl[i].arrivaltime;
    }


    /* Initialize system clock */
    l = 0;

    pthread_t cpu_id;	// CPU ID
    pthread_t loader_id;	// LOADER ID

    /* Initialize queues */
    initialize_queue(&in_queue);
    initialize_queue(&ready_queue);
    initialize_queue(&result_queue);

    /* Read a list of jobs to be run */
    int num_proc = 0;
    int k = 0;
    int j;
    int scanarrival, scanburst;
    int * arriv = (int *)malloc(sizeof(int));
    while((j = fscanf(ifp, "%d %d\n", &scanarrival, &scanburst)) != EOF) {
        struct pcb_t * proc =
                (struct pcb_t *)malloc(sizeof(struct pcb_t));
        proc->arrival_time = scanarrival;
        proc->burst_time = scanburst;
        arriv[k] = scanarrival;
        proc->timeslot_count = 0;
        proc->pid = k;
        proc->assigned_timeslot = malloc(sizeof(struct timeslot) * 10);
        en_queue(&in_queue, proc);
        k++;
        pl_size ++;
    }

    /* Start cpu */
    pthread_create(&cpu_id, NULL, cpu, NULL);
    /* Start loader */
    pthread_create(&loader_id, NULL, loader, NULL);

    /* Wait for cpu and loader */
    pthread_join(cpu_id, NULL);
    pthread_join(loader_id, NULL);


    l=0;
    ofp = fopen(ovalue, "w");
    if(ofp == NULL){
        perror ("ERROR: output file open failed\n");
        return (-1);
    }
    /* Produce output data */
    throughput[3] = 0, wait[3] = 0, response[3] = 0, turnaround[3] = 0;
    while (l < pl_size)
    {

        struct pcb_t * proc = de_queue(&result_queue);
        while (proc->pid != l)
        {
            en_queue(&result_queue,proc);
            proc = de_queue(&result_queue);
        }
        if(proc->timeslot_count == 1)
            wait[3] = wait[3] + proc->assigned_timeslot[0].starttime - arriv[l];
        else
        {
            int j;
            for (j = 0; j < proc->timeslot_count; j++) {
                if (j == 0)
                    wait[3] = wait[3] + proc->assigned_timeslot[0].starttime - arriv[l];
                else
                    wait[3] = wait[3] + proc->assigned_timeslot[j].starttime - proc->assigned_timeslot[j-1].endtime;
            }
        }
        turnaround[3] += proc->assigned_timeslot[proc->timeslot_count-1].endtime - arriv[l];
        response[3] += proc->assigned_timeslot[0].starttime - arriv[l];
        l++;
    }

   /* TODO: calculate scheduling criteria
             base on pl (=process_list) which is already existed */
   throughput[3] = (float)l / timecounter;
   int max_throughput = 0;
   int min_throughput = 9999;
   int max_wait = 0;
   int min_wait = 9999;
   int max_response = 0;
   int min_response = 9999;
   int max_turnaround = 0;
   int min_turnaround = 9999;
   int max_throughput_tag = 0;
   int min_throughput_tag = 0;
   int max_wait_tag = 0;
   int min_wait_tag = 0;
   int max_response_tag = 0;
   int min_response_tag = 0;
   int max_turnaround_tag = 0;
   int min_turnaround_tag = 0;
   for (i = 0; i <= 3; i++) {
	if (throughput[i] > max_throughput) {
        max_throughput = throughput[i];
        max_throughput_tag = i;
    }
	if (throughput[i] < min_throughput) {
        min_throughput = throughput[i];
        min_throughput_tag = i;
    }
	if (wait[i] > max_wait) {
        max_wait = wait[i];
        max_wait_tag = i;
    }	   
	if (wait[i] < min_wait) {
        min_wait = wait[i];
        min_wait_tag = i;
    }
	if (response[i] > max_response) {
        max_response = response[i];
        max_response_tag = i;
    }	   
	if (response[i] < min_response) {
        min_response = response[i];
        min_response_tag = i;
    }
	if (turnaround[i] > max_turnaround) {
        max_turnaround = turnaround[i];
        max_turnaround_tag = i;
    }	   
	if (turnaround[i] < min_turnaround) {
        min_turnaround = turnaround[i];
        min_turnaround_tag = i;
    }
   }
   printf("1: FCFS, 2: SJF, 3: SRTF, 4: RR\n");
   printf("Max throughput: %d\n", max_throughput_tag);
   printf("Min throughput: %d\n", min_throughput_tag);
   printf("Max wait: %d\n", max_wait_tag);
   printf("Min wait: %d\n", min_wait_tag);
   printf("Max response: %d\n", max_response_tag);
   printf("Min response: %d\n", min_response_tag);
   printf("Max turnaround: %d\n", max_turnaround_tag);
   printf("Min turnaround: %d\n", min_turnaround_tag);
   return 0;
}
