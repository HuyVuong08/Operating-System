#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_UNIT	100 // In microsecond
#define MAX_PROCESS	50

#include <pthread.h>

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



int main(int argc, char* argv[])
{
    char *ivalue = NULL;
    char *ovalue = NULL;
    char *qvalue = NULL;
    FILE *ifp, *ofp;
    struct pcb_t pl[MAX_PROCESS];
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
        ovalue = "/home/mercedes-a200/OS_Assignment/Output/output_RR.txt";
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

    /* Initialize system clock */


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
    float throughput = 0, wait = 0, response = 0, turnaround = 0;
    while (l < pl_size)
    {

        struct pcb_t * proc = de_queue(&result_queue);
        while (proc->pid != l)
        {
            en_queue(&result_queue,proc);
            proc = de_queue(&result_queue);
        }
        for(i = 0; i < proc->timeslot_count; i++)
            fprintf(ofp, "Process %2d start %5d end%5d\n", proc->pid,
                    proc->assigned_timeslot[i].starttime, proc->assigned_timeslot[i].endtime);

        if(proc->timeslot_count == 1)
            wait = wait + proc->assigned_timeslot[0].starttime - arriv[l];
        else
        {
            int j;
            for (j = 0; j < proc->timeslot_count; j++) {
                if (j == 0)
                    wait = wait + proc->assigned_timeslot[0].starttime - arriv[l];
                else
                    wait = wait + proc->assigned_timeslot[j].starttime - proc->assigned_timeslot[j-1].endtime;
            }
        }
        turnaround += proc->assigned_timeslot[proc->timeslot_count-1].endtime - arriv[l];
        response += proc->assigned_timeslot[0].starttime - arriv[l];
        l++;
    }

    /* TODO: calculate scheduling criteria
             base on pl (=process_list) which is already existed */
    throughput = (float)l / timecounter;
    fprintf(ofp, "Throughput time: %.2g\n", throughput);
    fprintf(ofp, "Average waiting time: %.2g\n", wait/(float)l);
    fprintf(ofp, "Average response time: %.2g\n", response/(float)l);
    fprintf(ofp, "Average turnaround time: %.2g\n", turnaround/ (float)l);
    return 0;
}


