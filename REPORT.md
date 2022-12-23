# xv6 - but better #
## Scheduling Algorithm 1: FCFS

> changes made:

- edited the _struct proc_ to store more data such as ctime, rtime, etime, wtime etc
- initialized the these new variables in the function _allocproc()_
- updated the functions _usertrap()_ and _kerneltrap()_ in _trap.c_ to disable interrupts. this was done because this scheduling algorithm is to be non preemptive
- implemented the changed scheduling algorithm in the function _scheduler()_ within _kernel/proc.c_
>implementation:

- if the FCFS flag is active, our fcfs scheduling algorithm is ran
- we iterate through all the processes stored in _proc[NPROC]_
- we acquire lock on each process
- if we find a process with lower creation time than the current one, we release the lock on the previous process and not on the current newly found process
- once a process is finalized, we change the state of the process to running and allocate this process to the cpu

## Scheduling Algorithm 2: LBS

> changes made:

- edited the _struct proc_ to store the number of tickets per process
- each process was given a ticket of 1. this was implemented in the _allocproc()_ function
- updated the functions _usertrap()_ and _kerneltrap()_ in _trap.c_ to let the interrupts on. as lbs is preemptive
- added a syscall **settickets**. this function allocates tickets to the current running process and change the previous number of tickets for it.
- implemented the changed scheduling algorithm in the function _scheduler()_ within _kernel/proc.c_
>implementation:

- if the LBS flag is active, our lbs scheduling algorithm is ran
- we iterate through all the processes stored in _proc[NPROC]_ and count the total number of tickets
- we then generate a _randomNum_
- we will again iterate through all processes and keep adding the number of tickets per process to a variable _ticketsSoFar_
- we also acquire lock on each process and release the lock if the process is not run.
- if the _ticketsSoFar_ >= (randomNum modulo totalTickets) + 1 and the current process is runnable, we run this process
-  we change the state of this process to running and allocate this process to the cpu

## Scheduling Algorithm 3: PBS

> changes made:

- edited the _struct proc_ to store priority and noOfRuns
- each process was given a priority of 60 and noOfRuns was set to 0. this was implemented in the _allocproc()_ function
- updated the functions _usertrap()_ and _kerneltrap()_ in _trap.c_ to disable interrupts. this was done because this scheduling algorithm is to be non preemptive
- added a syscall **set_priority**. this function takes in as arguments _pid_ and _priority_ and sets the _priority_ of the process with the given _pid_.
- implemented the changed scheduling algorithm in the function _scheduler()_ within _kernel/proc.c_
>implementation:

- if the PBS flag is active, our PBS scheduling algorithm is ran
- we iterate through all the processes stored in _proc[NPROC]_ and count the total number of tickets
- In our implementation, higher value implies lower priority.
- We store the first process as our finalProc.
- If a process has a lower priority value than finalProc, finalProc is set to the new process.
-  we change the state of this process to running and allocate this process to the cpu

## Scheduling Algorithm 4: MLFQ

> changes made:

- edited the _struct proc_ to store the current Waittime, flag to check if the process is present in the queue, queue number, queue ticks, and enter time.
- all of the the above variables were initialized to 0 for each processes this was implemented in the _allocproc()_ function
- updated the functions _usertrap()_ and _kerneltrap()_ in _trap.c_ to let the interrupts on. as lbs is preemptive
- implemented the changed scheduling algorithm in the function _scheduler()_ within _kernel/proc.c_
>implementation:

- if the MLFQ flag is active, our mlfq scheduling algorithm is ran
- We create a Queue struct and implement basic queue functionalities like pop, push, popFront.
- We iterate through all the processes stored in _proc[NPROC]_ and find one which is runnable and not present any queue and add it the priority queue 0
- If a process is present in the queue and has waiting time greater than the max age, we move it to a higher priority queue
- Now, in the scheduler we go from priority 0 to priority 4 queues and execute processes in order. 
- If the process relinquishes control of the CPU for any reason without using its entire timeslice, then we put it back in the same queue.
- Before execution it is popped from queue, in trap.c if we find a better priority process to preempt or the process is giving up control then we push it back into the same priority queue and yield cpu. Otherwise it has finished its time slice and we push it into the next queue.


## Performance Comparison

> the following results were obtained by running schedulertest using a single CPU


| Scheduler | runtime    | waittime    |
| :---:   | :---: | :---: |
| RoundRobbin | 15   | 162   |
| FCFS | 31   | 142   |
| Lotter Based | 15   | 157   |
| Priority Based | 15   | 131   |
| MLFQ | 15   | 162   |