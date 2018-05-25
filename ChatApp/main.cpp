#include "protos.h"


static int max_fd = 2;


int main(int argc, char** argv)
{
    SOCKET lsock;
    fd_set masterSet;
    struct addrinfo *result;
    struct sockaddr_storage *myAddr;
    
    
    if((result = addrget(TCP, SERVERMODE, NULL, PORT)) == NULL)
    {
        exit(1);
    }
    
    myAddr = (struct sockaddr_storage *) (result -> ai_addr);
    
    if((lsock = sockSetup(result)) == -1)
    {
        fprintf(stderr, "%s\n", "Cannot create listening Socket");
        exit(1);
    }
    
    max_fd++;
    FD_ZERO(&masterSet);
    FD_SET(lsock, &masterSet);
    
    //bind server socket
    if((bind(lsock, result->ai_addr, result->ai_addrlen)) == -1)
    {
        perror("bind");
        exit(1);
    }
    
    if((listen(lsock, LISTENQUEUE)) == -1)
    {
        perror("Listen");
        exit(1);
    }
    
    for(;;)
    {
        /* Main Loop */
        puts("Hey");
        break;
    }
    
    
    
    freeaddrinfo(result);
    
    close(lsock);
}
//END function main
//=====================================================================================
int sockSetup(struct addrinfo* ai)
{
    int sockfd;
    
    
    if (ai == NULL)
    {
        fprintf(stderr, "%s\n", "Cannot configure socket");
        return -1;
    }
    
    if ((sockfd = socket(ai -> ai_family, ai -> ai_socktype, ai -> ai_protocol)) == -1)
    {
        perror("Error on Socket Create");
        return -1;
    }
    
    return sockfd;
}
//END function sockSetup
//=====================================================================================
struct addrinfo* addrget(MODE protocol, MODE usage, const char* hostname, const char* port)
{
    int err;
    struct addrinfo hints, *result;
    
    
    if(usage != SERVERMODE && usage != CLIENTMODE)
    {
        fprintf(stderr, "%s\n", "Invalid Mode");
        return NULL;
    }
    
    
    if(hostname == NULL && port == NULL)
    {
        fprintf(stderr, "%s\n", "Cannot construct addrinfo without hostname OR port");
        return NULL;
    }
    
    
    if(hostname == NULL && usage == CLIENTMODE)
    {
        fprintf(stderr, "%s\n", "No client host specified");
        return NULL;
    }
    
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = (protocol == UDP) ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_flags = (usage == SERVERMODE) ? AI_PASSIVE : AI_CANONNAME;//get our IP for binding if server, otherwise get client name
    
    err = (usage == CLIENTMODE) ? (getaddrinfo(hostname, port, &hints, &result))
    : (getaddrinfo(NULL, port, &hints, &result)); //DNS lookup
    
    if(err != 0) //gai failure
    {
        gai_strerror(err);
        return NULL;
    }
    
    else // successful lookup
    {
        return result;
    }
}
//END function addrget
//=====================================================================================
