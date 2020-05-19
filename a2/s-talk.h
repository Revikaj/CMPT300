#define MYPORT "4950"
#define NUMTHREADS 4
#define INPUTBUFFERSIZE 1024
#define INCOMINGMSGSIZE 1024
#define PRINTSCREENMSGSIZE 1024


void initiateLists();

struct addrinfo *setupLocal(char *localPortNum);

struct addrinfo *setupRemote(char *remoteNameChar, char *remotePortNum);

void *awaitInput();

void *awaitUDP(void *remote);

void *printScreen();

void *sendUDP(void *remote);
