#include "protos.h"


static int max_fd = 2;


int main(int argc, char** argv)
{
    SOCKET lsock;
    fd_set masterSet;
    struct addrinfo *result;
    struct sockaddr_storage *myAddr;
    int tempMax;
    
    if((result = addrget(TCP, SERVERMODE, NULL, PORT)) == NULL)
    {
        perror("addrget");
        exit(1);
    }
    
    myAddr = (struct sockaddr_storage *) (result -> ai_addr);
    
    if((lsock = sockSetup(result)) == -1)
    {
        fprintf(stderr, "%s\n", "Cannot create listening Socket");
        exit(1);
    }
    
    max_fd = lsock;
    FD_ZERO(&masterSet);
    FD_SET(lsock, &masterSet);
    
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
        char buffer [BUFLEN];
        fd_set readers;
        int ret, fd_iter;
        
        readers = masterSet;
        memset(buffer, 0, sizeof buffer);
        ret = select(max_fd + 1, &readers, NULL, NULL, NULL);
        tempMax = max_fd;
        for(fd_iter = 0; fd_iter <= max_fd; fd_iter++)
        {
            if(FD_ISSET(fd_iter, &readers))
            {
                if(fd_iter == lsock) //connection coming on listening sock
                {
                    SOCKET newConn = accept(fd_iter, result->ai_addr,
                                            &result->ai_addrlen);
                    FD_SET(newConn, &masterSet);
                    if(tempMax < newConn)
                    {
                        tempMax = newConn;
                    }
                }
                else //we have a message
                {
                     if(recv(fd_iter, buffer, BUFLEN, 0) == -1)
                     {
                         exit(1);
                     }
                }
            }
        }
        max_fd = tempMax;
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
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = (protocol == UDP) ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_flags = (usage == SERVERMODE) ? AI_PASSIVE : AI_CANONNAME;//get our IP for binding if server, otherwise get client name
    
    err = (usage == CLIENTMODE) ? (getaddrinfo(hostname, port, &hints, &result))
    : (getaddrinfo(NULL, port, &hints, &result)); //DNS lookup
    
    if(err != 0) //gai failure
    {
        fprintf(stderr,"%s\n", gai_strerror(err));
        return NULL;
    }
    
    else // successful lookup
    {
        return result;
    }
}
//END function addrget
//=====================================================================================
