#ifndef PROCESS_HPP_
#define PROCESS_HPP_

struct timeslot
{
    int starttime; /* Timestamp at the start of execution */
    int endtime;    /* Timestamp at the    end of execution */
};

struct process
{
    bool isCheck = false;
    
    /* Values initialized for each process */
    int arrivaltime;    // Timestamp process arrives and wish to start
    int bursttime;      // An amount of time process requires to complete the job
    int burtbackup;     // Since during operations, bursttime will be modified, burstbackup backups data for later printing purpose
    /* Values algorithm may use to track process */
    struct timeslot* assigned_timeslot;
    int timeslot_count = 0;
    int timeslot_max;
    int flag;
};

#endif