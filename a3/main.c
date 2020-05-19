#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "main.h"



//Declare our ready queues
LIST * lowPriorityQueue;
LIST * normPriorityQueue;
LIST * highPriorityQueue;

//Declare our job queue (queue which holds the set of all processes in the system)
LIST * jobQueue;

//Declare our running process queue (for processes which are running)
LIST *runningProcessQueue;

//Declare our blocked queue (for processes which are blocked)
LIST *blockedQueue;

SEMAPHORE semQueue[5]; //Declare queue for semaphores.  Each semaphore id will correspond to its index in the queue

//PCB * initProcess;

unsigned int assignProcessId;

bool allProcessesBlocked = false;


void itemFree(void *ptr)
{
        ptr = NULL;
}


void initializeSimulation()
{
	printf("Initializing simulation...\n");

	//Initialize our ready queues
	lowPriorityQueue = ListCreate();
	normPriorityQueue = ListCreate();
	highPriorityQueue = ListCreate();

	//Initialize the queue which holds all processes
	jobQueue = ListCreate();

	//Initialize the queue which holds all running processes
	runningProcessQueue = ListCreate();

	//Initialize the queue which holds all blocked processes
	blockedQueue = ListCreate();

	//Initialize the init process
	PCB * initProcess = malloc(sizeof(PCB));
	initProcess->priority = 3; 
	initProcess->state = 1; 
	initProcess->id = 0; 
	initProcess->msg[0] = '\0';
	initProcess->awaitingMsg = false;
	initProcess->remoteId = -1;

	//init process will be the very first process in the job queue
	ListAppend(jobQueue, initProcess); 


	//init process will be the very first process that is running
        ListAppend(runningProcessQueue, initProcess);	


	//Initialize the variable which tracks what id to give to a process when it is created
	assignProcessId = 1;

	//Initialize semaphore queue
	for(int i = 0; i < 5; i++)
	{
		semQueue[i].id = -1;
		semQueue[i].value = -1;
		semQueue[i].processes = NULL;
	}



	printf("Simulation ready.\n\n");
	return;
}


void terminateSimulation()
{

	printf("Terminating simulation now...\n\n");	

	if(ListCount(lowPriorityQueue) > 0)
	{
		ListFree(lowPriorityQueue, itemFree);
	}

	
	if(ListCount(normPriorityQueue) > 0)
	{
		ListFree(normPriorityQueue, itemFree);
	}

	if(ListCount(highPriorityQueue) > 0)
	{		
		ListFree(highPriorityQueue, itemFree);
	}

	if(ListCount(jobQueue) > 0)
	{	
		ListFree(jobQueue, itemFree);
	}

	if(ListCount(runningProcessQueue) > 0)
	{
		ListFree(runningProcessQueue, itemFree);
	}

	if(ListCount(blockedQueue) > 0)
	{
		ListFree(blockedQueue, itemFree);
	}

	for(int i = 0; i < 5; i++)
	{
		semQueue[i].id = -1;
		semQueue[i].value = -1;
		if(semQueue[i].processes != NULL && ListCount(semQueue[i].processes) > 0)
		{
			ListFree(semQueue[i].processes, itemFree);
		}
	}	

	exit(1);

}

//Helper function which will migrate processes between the queues and set process priorities accordingly
void manageQueues()
{
	PCB * currProcess;

	//If there are no more processes with priority 2
	if(ListCount(highPriorityQueue) == 0)
	{
		//Migrate all the processes with priority 1 over to the high priority queue (change their priorites from 1 to 2)
		if(ListCount(normPriorityQueue) > 0)
		{
			currProcess = ListFirst(normPriorityQueue);
		
			//Migrate every process from the norm priority queue to the high priority queue
			while(ListCount(normPriorityQueue) != 0 && currProcess != NULL)
			{
				currProcess = ListFirst(normPriorityQueue);

				currProcess->priority = 2;

				ListAppend(highPriorityQueue, currProcess);
				
				ListRemove(normPriorityQueue);

			}

			//Migrate every process from the low priority queue to the norm priority queue
			if(ListCount(lowPriorityQueue) > 0)
			{
				currProcess = ListFirst(lowPriorityQueue);

				while(ListCount(lowPriorityQueue) != 0 && currProcess != NULL)
				{
					currProcess = ListFirst(lowPriorityQueue);
				
					currProcess->priority = 1;
	
					ListAppend(normPriorityQueue, currProcess);
	
					ListRemove(lowPriorityQueue);
				}
			}
		}

		//Migrate all the processes with priority 0 over to the high priority queue (change their priorities from 0 to 2)
		else if(ListCount(lowPriorityQueue) > 0)
		{
			currProcess = ListFirst(lowPriorityQueue);

			while(ListCount(lowPriorityQueue) != 0 && currProcess != NULL)
			{
				currProcess = ListFirst(lowPriorityQueue);

				currProcess->priority = 2;

				ListAppend(highPriorityQueue, currProcess);

				ListRemove(lowPriorityQueue);

			}

		}


	}

}

void runNewProcess()
{

	PCB * currProcess;

	
        if(ListCount(highPriorityQueue) > 0)
        {
		
        	currProcess = ListFirst(highPriorityQueue);
        	currProcess->state = 1;

        	ListAppend(runningProcessQueue, currProcess);
        	ListRemove(highPriorityQueue);
		printf("New process set to run...\n");
		printf("Process id currently running: %d with priority %d\n\n",currProcess->id, currProcess->priority);
		if(currProcess->awaitingMsg == true)
		{
			printf("Process has a message waiting for it.  Enter 'R' to receive the message\n\n");
		}
        }
        else if(ListCount(normPriorityQueue) > 0)
        {
                currProcess = ListFirst(normPriorityQueue);
                currProcess->state = 1;

                ListAppend(runningProcessQueue, currProcess);
        	ListRemove(normPriorityQueue);
		printf("New process set to run...\n");
                printf("Process id currently running: %d with priority %d\n\n",currProcess->id,currProcess->priority);
		if(currProcess->awaitingMsg == true)
		{
			printf("Process has a message waiting for it.  Enter 'R' to receive the message\n\n");
		}
        }
        else if(ListCount(lowPriorityQueue) > 0)
        {
                currProcess = ListFirst(lowPriorityQueue);
                currProcess->state = 1;

                ListAppend(runningProcessQueue, currProcess);
                ListRemove(lowPriorityQueue);
		printf("New process set to run...\n");
                printf("Process id currently running: %d with priority %d\n\n",currProcess->id,currProcess->priority);
		if(currProcess->awaitingMsg == true)
		{
			printf("Process has a message waiting for it.  Enter 'R' to receive the message\n\n");
		}
	}
	else
	{
		printf("Process currently running: INIT PROCESS\n");
		printf("runNewProcess: All ready queues are currently empty.  Please create a new process\n");
		printf("WARNING: Entering 'E' now will kill the init process and terminate the simulation\n\n");
	}

	manageQueues();



}

void enqueueProcess(PCB * process)
{
						
	if(process->priority == 0)
	{
		process->state = 0;
		ListAppend(lowPriorityQueue, process);
	}
	else if(process->priority = 1)
	{
		process->state = 0;
		ListAppend(normPriorityQueue, process);
	}
	else
	{
		process->state = 0;
		ListAppend(highPriorityQueue, process);
	}

}
void create(int priority)
{

	if(priority < 0 || priority > 2)
	{
		printf("Create: Failure, invalid priority value specified.  Priority value must be either 0, 1 or 2\n");
		return;
	}

	//Dynamically allocate memory for the pcb
	PCB * process  = malloc(sizeof(PCB));

	//Assign a priority to the process
	process->priority = priority;

	//Assign a process id to the process
        if(assignProcessId == MAX_PID_32)
	{
		process->id = assignProcessId;
		assignProcessId = 1;
	}
	else
	{
		process->id = assignProcessId;
		assignProcessId++;
	}

	//Assign an empty inbox to the newly created process
	process->msg[0] = '\0';

	//Newly created process will not be waiting for any messages
	process->awaitingMsg = false;	
	process->remoteId = -1;


	//If there is a process currently running, put the created process into its appropriate ready queue
	if(ListCount(runningProcessQueue) > 1)
	{
		PCB * currentlyRunning = ListLast(runningProcessQueue);
		printf("Process id currently running: %d\n",currentlyRunning->id);
		enqueueProcess(process);
	}
	//Else, if the only process running is the init process, run the newly created process
	else
	{
		PCB * initProcessQueued = ListFirst(runningProcessQueue);	
		process->state = 1;	
		initProcessQueued->state = 0;
		ListAppend(runningProcessQueue, process);
		printf("No processes currently running, created process set to run...\n");
		printf("Process id currently running: %d\n",process->id);
	

	}


	ListAppend(jobQueue, process);
	printf("Create: Success, process created.  Id of the new process: %d Priority: %d State: %d\n\n",process->id, process->priority, process->state);
	
}

void Fork()
{
	
	if(ListCount(runningProcessQueue) > 0)
	{
		
		//Get the currently running process
		PCB * runningProcess = ListLast(runningProcessQueue);
	
		//Check if the process is the init process	
		if(runningProcess->priority == 3 && runningProcess->id == 0)
		{
			printf("Fork: Failure, attempted to fork init process.\n\n");
			return;
		}

		//Create a copy of the running process
		PCB * processCopy = malloc(sizeof(PCB));

		//Copy the running process's information to the copy
		processCopy->priority = runningProcess->priority;	
		strcpy(processCopy->msg, runningProcess->msg);	//may have to change this

		//Assign a new id to the new process	
		processCopy->id = assignProcessId;
		assignProcessId++;

		//Enqueue process into its appropriate ready queue
		enqueueProcess(processCopy);

		//Append the process into the job queue
		ListAppend(jobQueue, processCopy);
	
		//success, report the copy process's id
		printf("Fork: Success, new process id: %d with priority %d\n\n",processCopy->id, processCopy->priority);
		
	}
	else
	{
		//No processes in the running queue.  Report failure.
		printf("Fork: Failure, no running processes available to fork.\n\n");
		

	}
}

void Kill(int pid)
{
	//Count will track the number of instances we remove a process from a queue
	int count = 0;
	bool killedCurrentlyRunning = false;

	//If the process we're going to kill is the init process, check if there are no other processes
	if(pid == 0)
	{
		if(ListCount(jobQueue) > 1)
		{
			printf("Kill: Failure, attempted to kill init process but there are other processes in the system\n");
			return;
		}
	}

	//Initialize process pointer to look for process to kill
	PCB * currProcess;	

	if(ListCount(runningProcessQueue) > 0)
	{
		
		currProcess = ListFirst(runningProcessQueue);
		//Traverse through the running process queue searching for a process with id = pid

		while(currProcess != NULL)
		{	
			if(currProcess->id == pid)
			{
				ListRemove(runningProcessQueue);
				count++;
				killedCurrentlyRunning = true;
				break;	
			}
			else
			{
	
				currProcess = ListNext(runningProcessQueue);
			}
		}
	}

	if(ListCount(jobQueue) > 0)
	{
		currProcess = ListFirst(jobQueue);
		//Traverse through the job queue searching for a process with id = pid
		while(currProcess != NULL)
		{
	
			if(currProcess->id == pid)
			{
				ListRemove(jobQueue);
				count++;
				break;
			}
			else
			{
				currProcess = ListNext(jobQueue);
			}
		}
	}

	if(ListCount(blockedQueue) > 0 && count == 1)
	{
		currProcess = ListFirst(blockedQueue);
		//Traverse through the blocked queue searching for a process with id = pid
		while(currProcess != NULL)
		{
			
			if(currProcess->id == pid)
			{
				ListRemove(blockedQueue);
				count++;
				break;
			}
			else
			{
				currProcess = ListNext(blockedQueue);
			}


		}



	}
	//Check the ready queues for instances of the process and remove the process
	if(pid != 0 && count == 1)
	{
	
		if(ListCount(lowPriorityQueue) > 0)
		{
			currProcess = ListFirst(lowPriorityQueue);
			//Traverse through the low priority queue searching for a process with id = pid
			while(currProcess != NULL)
			{
				
				if(currProcess->id == pid)
				{	
					ListRemove(lowPriorityQueue);
					count++;
					break;
				}
				else
				{	
					currProcess = ListNext(lowPriorityQueue);
				}
			}
		}

		if(ListCount(normPriorityQueue) > 0)
		{
			currProcess = ListFirst(normPriorityQueue);
			//Traverse through the norm priority queue searching for a process with id = pid
			while(currProcess != NULL)
			{
				
				if(currProcess->id == pid)
				{
					ListRemove(normPriorityQueue);
					count++;
					break;
				}
				else
				{
					currProcess = ListNext(normPriorityQueue);
				}
			}

		}

		if(ListCount(highPriorityQueue) > 0)
		{
			currProcess = ListFirst(highPriorityQueue);
			//Traverse through the high priority queue searching for a process with id = pid
			while(currProcess != NULL)
			{
				
				if(currProcess->id == pid)
				{
					ListRemove(highPriorityQueue);
					count++;
					break;
				}
				else
				{
					currProcess = ListNext(highPriorityQueue);
				}
			}
		}

	}
	
	if(count == 0)
	{
		printf("Kill: Failure, there was no process in the system with the specified id\n\n");
	}
	else
	{
		if(pid == 0)
		{
			printf("Kill: Success, killed the init process\n");
			printf("No more processes residing in the system, simulation will now terminate...\n\n");
			terminateSimulation();
		}
		else 
		{
			printf("Kill: Success, killed process with id: %d\n",pid);
			if(killedCurrentlyRunning == true)
			{
				runNewProcess();

			}
			else
			{
				currProcess = ListLast(runningProcessQueue);
				
				if(currProcess->id == 0)
				{
					printf("Process currently running: INIT PROCESS\n\n");
				}
				else
				{
					printf("Process id currently running: %d\n\n", currProcess->id);
				}
			}
		}
	}

	free(currProcess);	
}

void Exit()
{
	
	PCB * currProcess;

	if(ListCount(runningProcessQueue) > 0)
        {
		//Get the currently running process
                currProcess = ListLast(runningProcessQueue);
	
		//If the currently running process is the init process 
		if(currProcess->priority == 3 && currProcess->id == 0)
		{
				
			
			int currentlyRunningId = currProcess->id;

			//Check if there are other processes in the system
			if(ListCount(jobQueue) > 1)
			{
				printf("Failure: attempted to kill init process but there are other processes in the system \n");
				return;
			}
			else
			{
				//Remove all instances of the init process
				ListRemove(runningProcessQueue);

				currProcess = ListLast(jobQueue);
				
				if(currProcess->id == 0)
				{
					
					ListRemove(jobQueue);

					printf("Exit: Success, killed the init process\n");
					printf("No more processes residing in the system, simulation will now terminate...\n");
					terminateSimulation();
				}


			}
		}
		//If the currently running process is not the init process
		else if(currProcess->priority != 3 && currProcess->id != 0)
		{	
			int currentlyRunningId = currProcess->id;
			
			//Remove the process from the running queue
			ListRemove(runningProcessQueue);
			printf("Exit: Success, killed process with id: %d\n", currProcess->id);

			//Search for the process in the job queue and remove it
			currProcess = ListFirst(jobQueue);

			while(currProcess != NULL)
			{
				if(currProcess->id == currentlyRunningId)
				{
					free(currProcess);
					ListRemove(jobQueue);;
					break;
				}
				else
				{
					currProcess = ListNext(jobQueue);
				}

			}
			
			//run a new process
			runNewProcess();
		}
	}
}

void Quantum()
{
	PCB * currProcess;

	//Check if there is a process currently running
	if(ListCount(runningProcessQueue) > 0)
	{
		currProcess = ListLast(runningProcessQueue);
		
		//Check if currently running process is the init process, if so, simply set its state to ready but leave it in the queue
		if(currProcess->priority == 3 && currProcess->id == 0)
		{
			printf("Quantum: Failure, init process cannot be expired.  Please create a new process\n\n");

		}
		//If the currently running process is not the init process
		else if(currProcess->priority != 3 && currProcess->id != 0)
		{	
			//Set process state to ready and put it in the low priority queue (all processes which expire head directly to low priority queue)
			currProcess->state = 0;
			currProcess->priority = 0;

			ListAppend(lowPriorityQueue, currProcess);
			ListRemove(runningProcessQueue);
			
			printf("Quantum: Success, process id %d time expired\n",currProcess->id);

			//run a new process 
			runNewProcess();

		}

	}

}

void Send(int pid, char * msg)
{

	PCB * sourceProcess;
	PCB * recipientProcess;

	if(pid == 0)
	{
		printf("Send: Failure, cannot send a message to the init process\n\n");
		return;
	}

	if(ListCount(runningProcessQueue) > 0 && ListCount(jobQueue) > 0)
	{
		//Source process is the currently running process
		sourceProcess = ListLast(runningProcessQueue);

		//If the current process is sending a message to itself,
		if(sourceProcess->id == pid)
		{
			printf("Send: Failure, process cannot send a message to itself\n\n"); //Might have to change this
			return;
		}


		//Recipient process is somewhere in the job queue
		
		recipientProcess = ListFirst(jobQueue);
		
		while(recipientProcess != NULL)
		{
			
			//Found the process to send a message to
			if(recipientProcess->id == pid)
			{
				//If source process is init process (init can send but not receive)
				if(sourceProcess->id == 0 && recipientProcess->state == -1)
				{
					strcpy(recipientProcess->msg, msg);
					recipientProcess->remoteId = sourceProcess->id;
					recipientProcess->awaitingMsg = true;

					printf("Process currently running: INIT PROCESS\n");
					printf("Send: Success, message sent from init process to process id: %d\n",recipientProcess->id);
					printf("Process id: %d awaiting receival is currently blocked.  Must be unblocked before it can receive the message from init\n\n",recipientProcess->id);
				}

				//If the sender sends a message to a process which is blocked, blocked process cannot receive until it is unblocked 
				else if(sourceProcess->id != 0 && recipientProcess->state == -1 && recipientProcess->awaitingMsg == false)
				{
					strcpy(recipientProcess->msg, msg);
					recipientProcess->remoteId = sourceProcess->id;
					recipientProcess->awaitingMsg = true;

					sourceProcess->state = -1;
					ListAppend(blockedQueue, sourceProcess);
					ListRemove(runningProcessQueue);

					printf("Send: Success, message sent from process id: %d to process id: %d\n", sourceProcess->id, recipientProcess->id);
					printf("Sending process with id: %d is now blocked.  Process id: %d awaiting receival\n",sourceProcess->id, recipientProcess->id);
					printf("Process id: %d awaiting receival is currently blocked.  Must be unblocked before it can receive the message from process id: %d\n\n",recipientProcess->id, sourceProcess->id);
			
					runNewProcess();			
				}
				//If recipient process is blocked because it called R first before sender sent a message, then send a message to that process and put it on a ready queue
				else if(sourceProcess->id != 0 && recipientProcess->state == -1 && recipientProcess->awaitingMsg == true)
				{
					//Send message
					strcpy(recipientProcess->msg, msg);
					recipientProcess->remoteId = sourceProcess->id;
					
					//Block sending process
					sourceProcess->state = -1;
					ListAppend(blockedQueue, sourceProcess);
					ListRemove(runningProcessQueue);

					//Unblock the process and put it on a ready queue
					
					//Search the blocked queue for the recipient process
					if(ListCount(blockedQueue) > 0)
					{
						recipientProcess = ListFirst(blockedQueue);

						while(recipientProcess != NULL)
						{	
							//Found the recipient process
							if(recipientProcess->id == pid)
							{
								//Put it on a ready queue
								enqueueProcess(recipientProcess);	
								//Unblock the process
								ListRemove(blockedQueue);
								
								printf("Send: Success, message sent from process id: %d to process id: %d\n", sourceProcess->id, recipientProcess->id);
								printf("Sending process with id: %d is now blocked.  Process id: %d awaiting receival\n",sourceProcess->id, recipientProcess->id);
								printf("Process id: %d awaiting receival has been unblocked and is now waiting in the ready queue\n\n", recipientProcess->id);

								runNewProcess();

								break;
							}
						}
					}

				}
				//If the recipient process is not waiting for a message and the sender sends a message
				else if(sourceProcess->id != 0 && recipientProcess->state == 0)
				{

					//Store the message inside the recipients message box
					strcpy(recipientProcess->msg, msg);
					recipientProcess->remoteId = sourceProcess->id;	
					recipientProcess->awaitingMsg = true;
	
					//Block the sender
					sourceProcess->state = -1;
					ListAppend(blockedQueue, sourceProcess);
					ListRemove(runningProcessQueue);
		
					printf("Send: Success, messsage sent from process id: %d to process id: %d\n",sourceProcess->id, recipientProcess->id);
					printf("Sending process with id: %d is now blocked.  Process id: %d awaiting receival\n",sourceProcess->id, recipientProcess->id);
	
					runNewProcess();
				}
				//If the process is sending a message to itself
				else if(recipientProcess->state == 1)
				{
					printf("Send: Failure, processes cannot send messages to themselves\n\n");
				}
			
				
				return;
			}
			else
			{
				recipientProcess = ListNext(jobQueue);
			}

		}
		
		//Unable to find a process with specified id
		printf("Send: Failure, no process associated with the specified id to send a message to\n");
		return;

	}

}

void Receive()
{

	PCB * recipientProcess;

	if(ListCount(runningProcessQueue) > 0)
	{
		//Get the currently running process
		recipientProcess = ListLast(runningProcessQueue);	
	
		if(recipientProcess->id == 0)
		{
			printf("Process currently running: INIT PROCESS\n");
			printf("Receive: Failure, init process cannot receive messages\n\n");
			return;
		}
		else if(recipientProcess->id != 0)
		{
			printf("Process currently running: %d\n",recipientProcess->id);
		}

		//If message has been sent, receiving process keeps executing and message is displayed	
		if(recipientProcess-> awaitingMsg == true && recipientProcess->remoteId != -1)
		{
			//Report source of message and message text
			printf("Receive: Success, process id: %d received a message from process id: %d\n",recipientProcess->id, recipientProcess->remoteId);
			printf("Please enter 'Y' now to acknowledge the sent message\n");
			printf("Message received: %s\n\n", recipientProcess->msg);
			recipientProcess->awaitingMsg = false;

		}
		//If message has not been sent, receiving process gets blocked 
		else if(recipientProcess->awaitingMsg == false && recipientProcess->remoteId == -1)
		{
			printf("Receive: No message has been sent to process id: %d yet\n",recipientProcess->id);
			printf("Process id: %d will now be blocked until it receives a message\n\n",recipientProcess->id);

			//Block the receiving process
			recipientProcess->state = -1;
			recipientProcess->awaitingMsg = true;
			ListAppend(blockedQueue, recipientProcess);
			ListRemove(runningProcessQueue);

			//Run a new process
			runNewProcess();
			
			//need to add semaphores here		
		}
		else if(recipientProcess->awaitingMsg == false && recipientProcess->remoteId != -1)
		{
			//Need to reply to source process
			printf("Process has already received message.  Must acknowledge sender by replying.  Please enter 'Y'\n\n");



		}
	}

}

void Reply(int pid, char * msg)
{
	PCB * sourceProcess;
	PCB * recipientProcess;

	if(ListCount(runningProcessQueue) > 0)
	{
		//Get process who just received a message
		recipientProcess = ListLast(runningProcessQueue);
		
		if(recipientProcess->id == 0)
		{
			printf("Reply: Failure, init process cannot receive messages, thus, it cannot reply\n\n");
			return;
		}


		printf("Process currently running: %d\n",recipientProcess->id);


		if(recipientProcess->awaitingMsg == false && recipientProcess->remoteId == -1)
		{
			printf("Reply: Failure, Current running process has no process to acknowledge\n");
			return;
		}
		//If the process is not awaiting a message and has a remote ID, it needs to unblock the sender and reply
		else if(recipientProcess->awaitingMsg == false && recipientProcess->remoteId != -1)
		{
			sourceProcess = ListFirst(blockedQueue);

			//Search for the sender in the blocked queue
			if(ListCount(blockedQueue) > 0 && ListCount(jobQueue) > 0)
			{
				while(sourceProcess != NULL)
				{
					//Found the sender
					if(sourceProcess->id = recipientProcess->remoteId)
					{
						//If the source process's id does not match with the function call parameter, return failure
						if(sourceProcess->id != pid)
						{
							printf("Reply: Failure, process specified is not the original sender of the received message\n");
							return;
						}

						//Unblock the sender and put it in its appropriate ready queue	
						enqueueProcess(sourceProcess);	
						ListRemove(blockedQueue);
															
						//Deliver the reply
						
						strcpy(sourceProcess->msg, msg);
						sourceProcess->remoteId = recipientProcess->id;
						sourceProcess->awaitingMsg = true;
						printf("Reply: Success, process id: %d replied back to process id: %d\n",recipientProcess->id, sourceProcess->id);
						printf("Process id: %d unblocked and is now ready to run\n\n",sourceProcess->id);
						break;
					}
					else 
					{
						sourceProcess = ListNext(blockedQueue);
					}
				}	
			}

		}
	}	


}

void newSemaphore(int semaphore, int initVal)
{

	if(semaphore < 0 || semaphore > 4)
	{
		printf("newSemaphore: Failure, semaphore id must be between 0 and 4 inclusive\n\n");
		return;
	}

	//Semaphore has already been initialized, return an error
	if(semQueue[semaphore].id == semaphore)
	{
		printf("newSemaphore: Failure, semaphore with this id has already been initialized\n\n");
		return;
	}
	//Create the semaphore and add it to the semaphore queue
	else if(semQueue[semaphore].id == -1)
	{

		SEMAPHORE sem;
		sem.id = semaphore;
		sem.value = initVal;
		sem.processes = ListCreate();
		semQueue[semaphore] = sem;
	}

	printf("newSemaphore: Success, created a new semaphore with id: %d\n\n",semaphore);
}

void semaphoreP(int semaphore)
{
	
	if(semaphore < 0 || semaphore > 4)
	{
		printf("semaphoreP: Failure, semaphore id must be between 0 and 4 inclusive\n]n");
		return;
	}

	if(semQueue[semaphore].id == -1)
	{
		printf("semaphoreP: Failure, semaphore with this id does not exist, please create a semaphore\n\n");
		return;
	}

	PCB * process;
	
	//Search for running process
	if(ListCount(runningProcessQueue) > 0)
	{
		process = ListLast(runningProcessQueue);
		
		if(process->id == 0 && process->priority == 3)
		{
			printf("Process currently running: INIT PROCESS\n");
		}
		else
		{
			printf("Process id currently running: %d\n",process->id);		
		}
		
		//Block the process
		if(semQueue[semaphore].value <= 0 && process->id != 0 && process->priority != 3)
		{
			process->state = -1;
			process->blockedBy = semaphore;
			ListAppend(blockedQueue, process);
			ListRemove(runningProcessQueue);

			semQueue[semaphore].value--;
			printf("semaphoreP: Success, semaphore value was decremented, process id: %d was blocked\n",process->id);
			printf("semaphoreP: Value of semaphore id: %d is now: %d\n\n", semQueue[semaphore].id, semQueue[semaphore].value);

			runNewProcess();
		}
		//Did not block the process
		else
		{
			semQueue[semaphore].value--;
			printf("semaphoreP: Success, semaphore value was decremented.  No process was blocked\n");
			printf("semaphoreP: Value of semaphore id: %d is now: %d\n\n", semQueue[semaphore].id, semQueue[semaphore].value);
		}

	}
}

void semaphoreV(int semaphore)
{
	if(semaphore < 0 || semaphore > 4)
	{
		printf("semaphoreV: Failure, semaphore id must be between 0 and 4 inclusve\n");
		return;
	}

	if(semQueue[semaphore].id == -1)
	{
		printf("semaphoreV: Failure, semaphore with this id does not exist, please create a semaphore\n\n");
		return;
	}

	PCB * currentlyRunning;
	PCB * process;

	currentlyRunning = ListLast(runningProcessQueue);

	if(ListCount(runningProcessQueue) > 1)
	{
		printf("Process id currently running: %d\n",currentlyRunning->id);
	}
	else
	{
		printf("Process currently running: INIT PROCESS\n\n");
	}

	//Process blocked by semaphore residing in blocked queue
	if(ListCount(blockedQueue) > 0 && semQueue[semaphore].value < 0)
	{
		process = ListLast(blockedQueue);
		
		while(process != NULL)
		{
			//Found a process blocked by the semaphore
			if(process->blockedBy == semaphore)
			{
				
				//Unblock the process and enqueue it to a ready queue
				
				process->state = 0;
				process->blockedBy = -1;
				
				enqueueProcess(process);
				ListRemove(blockedQueue);
				semQueue[semaphore].value++;	

				if(currentlyRunning->id == 0 && currentlyRunning->priority == 3)
				{
					printf("Success, process id: %d with priority %d was unblocked on behalf of INIT PROCESS\n\n",process->id, process->priority);
				}
				else
				{
					printf("Success, process id: %d with priority %d was unblocked on behalf of currently running process id: %d\n", process->id, process->priority, currentlyRunning->id);
					printf("Semaphore id %d value is now %d\n\n",semQueue[semaphore].id, semQueue[semaphore].value);
				}

				if(ListCount(runningProcessQueue) == 1)
				{
					runNewProcess();
				}

				return;
							
			}
			else
			{
				process = ListPrev(blockedQueue);
			}
		}	
	}
	else if(ListCount(blockedQueue) > 0)
	{
		semQueue[semaphore].value++;
		printf("Success, no process is blocked by semaphore id: %d so no process was unblocked\n",semQueue[semaphore].id);
		printf("Semaphore id: %d value is now %d\n\n", semQueue[semaphore].id, semQueue[semaphore].value);
	}
	else
	{
		semQueue[semaphore].value++;
		printf("Success, semaphore id: %d value is now %d\n\n", semQueue[semaphore].id, semQueue[semaphore].value);
	}
}

void Procinfo(int pid)
{
	PCB * process;

	//Search for the process
	if(ListCount(jobQueue) > 0)
	{
		process = ListFirst(jobQueue);

		while(process != NULL)
		{
			//Found the process
			if(process->id == pid)
			{
				if(process->id == 0)
				{
					printf("***INIT PROCESS***\n\n\n");
				}

				printf("Process ID   : %d\n",process->id);

				if(process->state == -1)
				{
					printf("Process State: BLOCKED\n");
					printf("Blocked By Semaphore ID: %d\n",process->blockedBy);
				}

				else if(process->state == 0)
				{
					printf("Process State: READY\n");
				}

				else
				{
					printf("Process State: RUNNING\n");
				}

				
				if(process->priority == 0  && process->state != -1)
				{
					printf("Process Priority: LOW\n");
					printf("Process Located in Low Priority Queue\n\n");
				}

				else if(process->priority == 1 && process->state != -1)
				{
					printf("Process Priority: NORMAL\n");
					printf("Process Located in Normal Priority Queue\n\n");
				}

				else if(process->priority == 2 && process->state != -1)
				{
					printf("Process Priority: HIGH\n");
					printf("Process Located in High Priority Queue\n\n");
				}
				else if(process->priority == 0 && process->state == -1)
				{
					printf("Process Priority: LOW\n");
					printf("Process Located in Blocked Queue\n\n");
				}
				else if(process->priority == 1 && process->state == -1)
				{
					printf("Process Priority: NORMAL\n");
					printf("Process Located in Normal Priority Queue\n\n");
				}
				else if(process->priority == 2 && process->state == -1)
				{
					printf("Process Priority: HIGH\n");
					printf("Process Located in High Priority Queue\n\n");
				}
				else	
				{
					printf("Process Priority: INIT\n");
					printf("Process currently running and not in any ready queues\n\n");
				}

				if(process->awaitingMsg == true && process->remoteId != -1)
				{
					printf("Process currently awaiting message: %s\n",process->msg);
					printf("Message source: Process %d\n\n",process->remoteId);
				}

				return;
			}
			else
			{
				process = ListNext(jobQueue);
			}
		}

		printf("Procinfo: Failure, no process located with the specifed id.  Please create a process\n\n");
	}

}

void Totalinfo()
{
	PCB * process;

	if(ListCount(runningProcessQueue) > 0)
	{
		process = ListLast(runningProcessQueue);
		if(process->id == 0)
		{
			printf("Process currently running: INIT PROCESS\n\n");
		}
		else
		{
			printf("Process currently running: %d\n\n",process->id);
		}
	}

	if(ListCount(lowPriorityQueue) > 0)
	{
		process = ListFirst(lowPriorityQueue);
		printf("LOW PRIORITY QUEUE CONTENTS: \n\n");

		while(process != NULL)
		{
			printf("Process ID: %d\n",process->id);
			process = ListNext(lowPriorityQueue);
		}
		printf("\n");
	}
	else
	{
		printf("Low Priority Queue is empty\n");
	}


	if(ListCount(normPriorityQueue) > 0)
	{
		process = ListFirst(normPriorityQueue);
		printf("NORMAL PRIORITY QUEUE CONTENTS: \n\n");

		while(process != NULL)
		{
			printf("Process ID: %d\n",process->id);
			process = ListNext(normPriorityQueue);
		}
		printf("\n");
	}
	else
	{
		printf("Norm Priority Queue is empty\n");
	}

	if(ListCount(highPriorityQueue) > 0)
	{
		process = ListFirst(highPriorityQueue);
		printf("HIGH PRIORITY QUEUE CONTENTS: \n\n");

		while(process != NULL)
		{
			printf("Process ID: %d\n",process->id);
			process = ListNext(highPriorityQueue);
		}
		printf("\n");
	}
	else
	{
		printf("High priority Queue is empty\n");
	}

	if(ListCount(blockedQueue) > 0)
	{
		process = ListFirst(blockedQueue);
		printf("BLOCKED QUEUE CONTENTS: \n\n");

		while(process != NULL)
		{
			printf("Process ID: %d\n",process->id);
			process = ListNext(blockedQueue);
		}
		printf("\n");
	}
	else
	{
		printf("Blocked Queue is empty\n");
	}



}

int main()
{
	initializeSimulation();

	int lowPriority = 0;
	int normPriority = 1;
	int highPriority = 2;

	printf("Command Legend: \n\n");
	                     
        printf("C: Create Process\n");
        printf("F: Fork Process\n");
        printf("K: Kill Process\n");
        printf("E: Exit Process\n");
        printf("Q: Expire time quantum process\n");
        printf("S: Send a process a message\n");
	printf("R: Receive a message from process\n");
	printf("Y: Reply to a message from a process\n");
	printf("N: Create a new semaphore\n");
	printf("P: Execute P semaphore operation on behalf of current process\n");
	printf("V: Execute V semaphore operation on behalf of current process\n");
	printf("I: Display all the information of the current process\n");
	printf("T: Display contents of all the process queues\n");
	printf("!: Exit simulation \n\n");

	while(1)
	{	
		char command;
		printf("Enter a command: ");
		scanf(" %c", &command);
		if(command == 'C')
		{
			create(lowPriority);
		}
		else if(command == 'F')
		{
			Fork();
		}
		else if(command == 'K')
		{
			Kill(1);
		}
		else if(command == 'E')
		{
			Exit();
		}
		else if(command == 'Q')
		{
			Quantum();
		}
		else if(command == 'S')
		{
			Send(2, "yo");
		}
		else if(command == 'R')
		{
			Receive();
		}
		else if(command == 'Y')
		{
			Reply(1, "Acknowledged");
		}
		else if(command == 'N')
		{
			newSemaphore(0, 1);
		}
		else if(command == 'P')
		{
			semaphoreP(0);
		}
		else if(command == 'V')
		{
			semaphoreV(0);
		}
		else if(command == 'I')
		{
			Procinfo(2);
		}
		else if(command == 'T')
		{
			Totalinfo();
		}
		else if(command == '!')
		{
			terminateSimulation();
		}
		else
		{
			printf("Invalid Command.  Please enter a valid command\n");
		}
	}



	return 0;
}
