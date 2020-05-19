Kevin San Gabriel
301342241
CMPT 300 Spring 2020
Assignment 3

Design Decisions:

    I created ready queues of 3 different levels.  Low, normal and high.  I also created a jobQueue which consisted of every single process in the system including the init process.  Finally, I created a blocked queue for any processes which were blocked and a running queue for processes that were running.

    The running queue was designed to only hold at most 2 processes.  The init process would always reside in the ready queue as the head item of the queue.  Any "normal" process that was set to execute would be appended after the init process and would be the new executing process

    Anytime a process's time quantum expired (the init process excluded), the process would be added to the hierarchy of ready queues.  I chose every process that expired to be added to the low priority queue.  I did this in consideration of the order of execution of the processes.  Processes which expired first, would go to the back of the line of all the processes waiting to get executed.  This is important because it gives all the processes an equal amount of time with the CPU and doesnt allow one process to hoard the CPU.  It also maximizes CPU efficiency because as soon as a process's time expires, a new process will be chosen starting with the high priority queue to execute.

    I did this by designing a process migration method for the ready queues.  The process migration method would migrate processes between the queues such that every time the high priority queue is exhausted of processes, processes from the normal priority queue would have their priority increased and they would migrate to the high priority queue.  This would apply also from the low priority queue to the high priority queue.

 
 
