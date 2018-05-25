//
//  protos.h
//  ChatApp
//
//  Created by Chris Beser on 5/24/18.
//  Copyright © 2018 Chris Beser. All rights reserved.
//

#ifndef protos_h
#define protos_h

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
#include <sys/types.h>
#include <sys/select.h>

#define PORT "7878"
#define SERVERMODE 0
#define CLIENTMODE 1
#define BUFLEN 4096
#define LISTENQUEUE 30
#define TCP 0
#define UDP 1


typedef int SOCKET;
typedef int MODE;
typedef unsigned int STAMP;

struct Message
{
    char msg_string[BUFLEN];
    //timestamp
};

struct LogEntry
{
    //MESSAGE STRUCT, with metadata
    struct Message msg;
    STAMP order_stamp;  //Index in Array
};

//Global Log Structure
//TODO



//END DEF
SOCKET sockSetup(struct addrinfo* ai);

/* Sets up socket based on addrinfo */

struct addrinfo* addrget(MODE protocol, MODE usage, const char* hostname, const char* port);
/* Returns linked list from getaddrinfo() */

#endif /* protos_h */
