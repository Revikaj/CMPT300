#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>
#include "s-talk.h"
#include "list.h"


//activeSession will control the concurrently running threads
bool activeSession = false;

//These variables will help control thread use of the condition variables
bool sendListEmpty = true;
bool printListEmpty = true;

int socketFD;

//Lists that will act as message buffers
LIST *sendList;
LIST *printScreenList;


//Storage to hold our thread IDs
pthread_t threadBuffer[NUMTHREADS];

//Mutexes which will solve the critical section problem
pthread_mutex_t printListLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sendListLock = PTHREAD_MUTEX_INITIALIZER;


//Condition variables
pthread_cond_t sendListFullCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t printListFullCond = PTHREAD_COND_INITIALIZER;

pthread_cond_t printListEmptyCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t sendListEmptyCond = PTHREAD_COND_INITIALIZER;

//Helper function for ListFree
void itemFree(void *ptr)
{
        ptr = NULL;
}

//Initiate our lists which will act as message buffers for our threads
void initiateLists()
{
	sendList = ListCreate();
	printScreenList = ListCreate();
}

//Reference for retrieving local and remote user address information: http://beej.us/guide/bgnet/

//Initiate the local computer for an s-talk session by getting its addrinfo, creating a socket and binding it to a port 
struct addrinfo *setupLocal(char *localPortNum)
{
	int returnVal;
	int localPort;
	char localPortC[6];
	struct addrinfo hints, *localInfo, *pointer;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	localPort = atoi(localPortNum);
	strcpy(localPortC, localPortNum);

	returnVal = getaddrinfo(NULL, localPortC, &hints, &localInfo);
	
	if (returnVal != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnVal));
	}

        for(pointer = localInfo; pointer != NULL; pointer = pointer->ai_next)
        {
                socketFD = socket(pointer->ai_family, pointer->ai_socktype, pointer->ai_protocol);
                if(socketFD == -1)
                {
                        perror("s-talk: socket creation failed");
                        continue;
                }

                if (bind(socketFD, pointer->ai_addr, pointer->ai_addrlen) == -1)
                {
                        close(socketFD);
                        perror("s-talk: socket bind failed");
                        continue;
                }
		
		if(pointer->ai_addr->sa_family == AF_INET)
		{
	
			freeaddrinfo(localInfo);
			return pointer;
		}
                break;
        }

        if (pointer == NULL)
        {
                fprintf(stderr, "s-talk: socket failed to bind");
        }

	return NULL;	

}

//Initialize the remote user's information so that we can send data to them
struct addrinfo *setupRemote(char *remoteNameChar, char *remotePortNum)
{
	int returnVal;
	int remotePort;
	char remotePortC[6];
	char remoteName[30];

	struct addrinfo hints, *remoteInfo, *pointer;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	remotePort = atoi(remotePortNum);
	strcpy(remotePortC, remotePortNum);
	strcpy(remoteName, remoteNameChar);


	returnVal = getaddrinfo(remoteName, remotePortC, &hints, &remoteInfo);

	if(returnVal != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnVal));
	}
	
	//This part prints out the IP address of the remote user given their remote machine name for debugging purposes
	for(pointer = remoteInfo; pointer != NULL; pointer = pointer->ai_next)
        {
                if(pointer->ai_addr->sa_family == AF_INET)
                {
                        struct sockaddr_in *sin = (struct sockaddr_in *) pointer->ai_addr;
                        char *ip = inet_ntoa(sin->sin_addr);
                        printf("IP ADDRESS OF REMOTE: %s\n", ip);
			freeaddrinfo(remoteInfo);
			return pointer;
                }
        }

	
}

//Thread 1 and thread 4 share the sendList list.
//Thread 2 and 3 share the printList list

//Reference used for cancellation points: https://stackoverflow.com/questions/34377260/pthread-cancel-and-cancellation-point
//Reference used for condition variables and mutex usage; https://stackoverflow.com/questions/23400097/c-confused-on-how-to-initialize-and-implement-a-pthread-mutex-and-condition-vari


/*Thread 1: Awaits input from the user.  If the user inputs "!", the session is terminated.*/
void *awaitInput()
{
	
	//Initialize buffer for user input.
	char inputBuffer[INPUTBUFFERSIZE];

	//Specify the character which terminates an s-talk session.
	char terminateFlag = '!';	
	
	//Cancellation point
	pthread_testcancel();

	while(activeSession)
	{
		pthread_testcancel();

		pthread_mutex_lock(&sendListLock);

		while(!sendListEmpty)
		{
			//Setup cancellation point
			pthread_testcancel();

			//While the sendList is not empty, wait for thread 4 to send a UDP datagram to the remote user
			pthread_cond_wait(&sendListEmptyCond, &sendListLock);
	
		}

		while(sendListEmpty)
		{	
			//Cancellation point
			pthread_testcancel();

			//Take input from the user and put that into inputBuffer
			printf("Enter a message: \n");
			memset(inputBuffer, '\0', INPUTBUFFERSIZE);
			fgets(inputBuffer, INPUTBUFFERSIZE, stdin);
			inputBuffer[strlen(inputBuffer)-1] = '\0';
	
			//If the user inputs the terminate character, terminate the session
			if(*inputBuffer == terminateFlag)
			{
				//Terminate the session
			
				printf("! inputted.  Terminating session now...\n");

				//Sleep the thread for a bit
				sleep(1);

				//Send last message containing ! to the receiver
				ListAdd(sendList,inputBuffer);
				sendListEmpty = false;
				pthread_mutex_unlock(&sendListLock);
				pthread_cond_signal(&sendListFullCond);
				
				//Cancel the sending thread		
				pthread_cancel(threadBuffer[3]);
			
				//Close the socket
				activeSession = false;			
				memset(inputBuffer, '\0', INPUTBUFFERSIZE);
		       		close(socketFD);

				
				//Free the lists
				if(ListCount(sendList) > 0)
				{
					ListFree(sendList, itemFree);
				}

				if(ListCount(printScreenList) > 0)
				{
					ListFree(printScreenList, itemFree);
				}
			
				
				//Destroy mutexes and condition variables
				pthread_mutex_unlock(&printListLock);
				pthread_mutex_unlock(&sendListLock);
			
				pthread_mutex_destroy(&printListLock);
				pthread_mutex_destroy(&sendListLock);

				pthread_cond_destroy(&sendListFullCond);
				pthread_cond_destroy(&printListFullCond);

				pthread_cond_destroy(&sendListEmptyCond);
				pthread_cond_destroy(&printListEmptyCond);

				//Cancel the receiver and screen threads
			
				pthread_cancel(threadBuffer[1]);
				pthread_cancel(threadBuffer[2]);
	
				
				//Exit keyboard thread
				pthread_exit(0);
				
			}	
			//Add user input to the sendList and signal the send thread (thread 4) to process the list
			else
			{
				ListAdd(sendList, inputBuffer);	
				sendListEmpty = false;
				pthread_mutex_unlock(&sendListLock);
				pthread_cond_signal(&sendListFullCond);		
			}
		}

		
	}
	
	

	pthread_exit(0);

}



/*Thread 2: Awaits a UDP datagram from the remote user.*/
void *awaitUDP(void *remote)
{

	int byte_count;
	char incomingMsgBuffer[INCOMINGMSGSIZE];
	struct sockaddr *remoteAddr = (struct sockaddr *)remote;
	int remoteLen = sizeof(remote);	
	char terminateFlag = '!';


	pthread_testcancel();

	while(activeSession)
	{
		pthread_testcancel();

		pthread_mutex_lock(&printListLock);		
	
		while(!printListEmpty)
		{
			//Setup cancellation point
			pthread_testcancel();
			
			//While the print list is empty, wait for thread 3 to process received data from the remote user
			pthread_cond_wait(&printListEmptyCond, &printListLock);
		}		
		
		
		while(printListEmpty)
		{
			//Cancellation point
			pthread_testcancel();
			
			//Check if we successfully received data from the remote user
			if( byte_count == recvfrom(socketFD, incomingMsgBuffer, INCOMINGMSGSIZE-1, 0, remoteAddr, &remoteLen) == -1)
			{
				perror("recvfrom: could not receive UDP datagram from remote user");
				exit(1);
			}
			//If we received the terminate character from the rmeote user, terminate the session.
			else if(*incomingMsgBuffer == terminateFlag)
			{
				//Terminate the session
				
				printf("! received from remote user.  Terminating session now...\n");
				
				//Sleep the thread for a bit
				sleep(1);		


                                //Close the socket
                                activeSession = false;
                                memset(incomingMsgBuffer, '\0', INPUTBUFFERSIZE);
                                close(socketFD);
				
				
                                //Free the lists
                                if(ListCount(sendList) > 0)
                                {
                                        ListFree(sendList, itemFree);
                                }

                                if(ListCount(printScreenList) > 0)
                                {
                                        ListFree(printScreenList, itemFree);
                                }


                                //Destroy mutexes and condition variables
                                pthread_mutex_unlock(&printListLock);
                                pthread_mutex_unlock(&sendListLock);

				pthread_mutex_destroy(&printListLock);
				pthread_mutex_destroy(&sendListLock);

                                pthread_cond_destroy(&sendListFullCond);
                                pthread_cond_destroy(&printListFullCond);

                                pthread_cond_destroy(&sendListEmptyCond);
                                pthread_cond_destroy(&printListEmptyCond);

                                //Cancel the input and sending threads
                                pthread_cancel(threadBuffer[0]);
                                pthread_cancel(threadBuffer[2]);
				pthread_cancel(threadBuffer[3]);
				
				//Exit receive thread
                                pthread_exit(0);
				

			}
			//Add the remote user's message to the printScreenList and signal thread 3 to process that message and print it out to the screen
			else
			{

				ListAdd(printScreenList, incomingMsgBuffer);	
				printListEmpty = false;
				pthread_mutex_unlock(&printListLock);
				pthread_cond_signal(&printListFullCond);
			}

		}
	}


	
	
	pthread_exit(0);

}


/*Thread 3: Prints characters to the screen.  This thread will access the printScreenList list*/

void *printScreen()
{
	char msgBuffer[PRINTSCREENMSGSIZE];
	
	//Cancellation point
	pthread_testcancel();
	
	while(activeSession)
	{
		pthread_testcancel();

		pthread_mutex_lock(&printListLock);
		
		while(printListEmpty)
		{
			//Setup cancellation point if another thread calls pthread_cancel
			pthread_testcancel();

			//If the printScreenList is empty wait for the receiving thread to receive a UDP datagram from the remote user
			pthread_cond_wait(&printListFullCond, &printListLock);

		}

		while(!printListEmpty)
		{
			//Cancellation point
			pthread_testcancel();

			
			if(ListCount(printScreenList) > 0)
			{
				if(ListFirst(printScreenList) != NULL && ListCurr(printScreenList) != NULL)
				{
					//Print remote user's message onto the screen and signal the receiving thread that its their turn to wait for a UDP datagram again
					strcpy(msgBuffer, ListFirst(printScreenList));
					ListRemove(printScreenList);
					pthread_mutex_unlock(&printListLock);
					pthread_cond_signal(&printListEmptyCond);
					printListEmpty = true;
					printf("msg received: %s\n", msgBuffer);
				}
			}
		}
	
		
	}
	
	
	
	pthread_exit(0);

}

/*Thread 4: Send UDP datagram to remote UNIX process over the network*/

void *sendUDP(void *remote)
{
	char msgBuffer[INPUTBUFFERSIZE];
	struct sockaddr *remoteAddr = (struct sockaddr *)remote;
	socklen_t remoteLength = sizeof(struct sockaddr_in);
	int byte_count;	
	
	//Cancellation point
	pthread_testcancel();	
		
	while(activeSession)
	{
		pthread_testcancel();	
	
		pthread_mutex_lock(&sendListLock);
		
		while(sendListEmpty)
		{
			//Setup cancellation point
			pthread_testcancel();
			
			//If the sendList is empty, wait for thread 1 to receive input from the local user
			pthread_cond_wait(&sendListFullCond, &sendListLock);
		}	
		
		while(!sendListEmpty)
		{	
			//Cancellation point
			pthread_testcancel();
			
			if(ListCount(sendList) > 0)
			{
			
				if(ListFirst(sendList) != NULL && ListCurr(sendList) != NULL)
				{
					//Send the local user's input to the remote user and signal thread 1 that its their turn to wait for input from the local user again
					strcpy(msgBuffer, ListFirst(sendList));
					ListRemove(sendList);
					sendListEmpty = true;
					pthread_mutex_unlock(&sendListLock);
					pthread_cond_signal(&sendListEmptyCond);						
					pthread_mutex_unlock(&sendListLock);

					//If the message failed to send, print an error message
					if(byte_count = sendto(socketFD, msgBuffer, INPUTBUFFERSIZE, 0, remote, remoteLength) == -1)
					{
						perror("sendUDP: failed to send UDP datagram to remote process");
						exit(1);
		
					}
			
				}
			}
			
		}
	}
		


	pthread_exit(0);

}





int main(int argc, char *argv[])
{
	
	int localPort;
	int remotePort;

	struct addrinfo AI_localAddr, AI_remoteAddr;
 
	struct sockaddr_in localAddr; //Our address which will be supplied to the other user
	struct sockaddr_in remoteAddr; //Address of the user we are communicating with

	//Check if the user executed the program correctly with the correct parameters.  
	if (argc != 4)
	{
		fprintf(stderr,"usage: s-talk myPortNumber remoteMachine remotePortNumber\n");
		exit(1);
	}
	
	//Initiate our lists
	initiateLists();


	localPort = atoi(argv[1]);
	remotePort = atoi(argv[3]);



	//Get the addrinfo for local machine, create a socket then bind it to a port
	
	AI_localAddr = *setupLocal(argv[1]);

	//Get addrinfo for the remote machine
	
	AI_remoteAddr = *setupRemote(argv[2],argv[3]);

	
	localAddr = *(struct sockaddr_in *) AI_localAddr.ai_addr;
	remoteAddr = *(struct sockaddr_in *) AI_remoteAddr.ai_addr;

	activeSession = true;

	//Create our threads
	pthread_create(&threadBuffer[0], NULL, awaitInput, NULL);
	pthread_create(&threadBuffer[1], NULL, awaitUDP, (struct sockaddr *)&remoteAddr);
	pthread_create(&threadBuffer[2], NULL, printScreen, NULL);
	pthread_create(&threadBuffer[3], NULL, sendUDP, (struct sockaddr *)&remoteAddr);

	//Pause the main function until the threads exit
	pthread_join(threadBuffer[0], NULL);
	pthread_join(threadBuffer[1], NULL);
	pthread_join(threadBuffer[2], NULL);
	pthread_join(threadBuffer[3], NULL);
	
	close(socketFD);
	
	exit(1);
}



















