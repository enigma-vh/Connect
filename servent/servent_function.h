//
//  servent_function.h
//  Project2
//
//  Created by Van Hao On on 4/14/20.
//  Copyright © 2020 Van Hao On. All rights reserved.
//

#ifndef servent_function_h
#define servent_function_h

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>



#include<sys/types.h>
#include<sys/un.h>
#include<netdb.h>
#include<ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "time.h"


#define NET_BUF_SIZE 512
#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"

// function to clear buffer
void clearBuf(char* b);

struct thread_arg{
    int* argc;
    char** argv;
};

void directory(void* arg);

void* publish(void* arg);

//void* clientThread(struct thread_arg* t);

void* send_message(void* arg);

void* search_file(void* arg);

#endif /* servent_function_h */
