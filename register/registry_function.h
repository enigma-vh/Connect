//
//  registry_function.h
//  Project2
//
//  Created by Van Hao On on 4/14/20.
//  Copyright © 2020 Van Hao On. All rights reserved.
//

#ifndef registry_function_h
#define registry_function_h

#include <stdio.h>

#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include "time.h"


#include <fcntl.h> // for open
#include<pthread.h>



#define NET_BUF_SIZE 512
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    int socket;
    int client_port_no;
    int c_id;
}client_id;

void clearBuf(char* b);

int exists_test(char* name);

void* receive_file(void* arg);

void * socketThread(void* arg);

void* transfer_file(void* arg);


#endif /* registry_function_h */
