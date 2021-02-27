//
//  client_test.c
//  Project2
//
//  Created by Van Hao On on 4/4/20.
//  Copyright © 2020 Van Hao On. All rights reserved.
//


#include <stdio.h>
#include "servent_function.h"

int main(int argc, char** argv){
    struct thread_arg t;
    t.argc = &argc;
    t.argv = argv;
    
    int client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, my_addr;
    
    if (client < 0)
        printf("Error in client creating\n");
    else
        printf("Client Created\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    int con = connect(client, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (con == 0){
        printf("Client Connected\n");
    }
    else{
        printf("Error in Connection\n");
        return 0;
    }

    int port_no = atoi(argv[3]);
    //directory(&client);
    //while(1){
        send(client, argv[3],strlen(argv[3]),0);
    
        pthread_t message_send, file_send, file_search;
        pthread_create(&message_send, NULL, send_message, &port_no);
    
        pthread_create(&file_send, NULL,publish, &client);
        sleep(1);
        pthread_create(&file_search, NULL, search_file, &client);

        pthread_join(message_send,NULL);
        pthread_join(file_send,NULL);
        pthread_join(file_search, NULL);

    return 0;
}








