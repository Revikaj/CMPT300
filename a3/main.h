#define PCB_MSG_BUFFER 41
#define MAX_PID_32 32768
#define MAX_PID_64 4194304

typedef struct PCB
{
	int id;
	int remoteId;
	int priority;
	int blockedBy;
	bool awaitingMsg;
	int state; //states are:  -1: Blocked; 0: Ready; 1: Running
	char msg[PCB_MSG_BUFFER];
}PCB;

typedef struct SEMAPHORE
{
	int id;
	int value;
	LIST * processes;


}SEMAPHORE;
