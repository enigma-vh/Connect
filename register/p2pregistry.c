//
//  server_test.c
//  Project2
//
//  Created by Van Hao On on 4/4/20.
//  Copyright © 2020 Van Hao On. All rights reserved.
//

#include <stdio.h>
#include "registry_function.h"

int main(int argc, char **argv)
{
    fd_set fds, readfds;
    int acc;
    int clientsock[2], rc, numsocks = 0, maxsocks = 2;
    int server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0)
        printf("Error in server creating\n");
    else
        printf("Server Created\n");

    struct sockaddr_in server_addr, client_addr;

    //memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        printf("Binded Correctly\n");
    else
        printf("Unable to bind\n");

    if (listen(server, 3) == 0)
        printf("Listening ...\n");
    else
        printf("Unable to listen\n");

    socklen_t addr_size;
    addr_size = sizeof(client_addr);
    FD_ZERO(&fds);
    FD_SET(server, &fds);
    while (1)
    {
        
        readfds = fds;
        rc = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

        if (rc == -1)
        {
            perror("Select");
            break;
        }
        
       
        pthread_t message_receive, file_receive, file_search;
        
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                if (i == server)
                {
                    while(numsocks < maxsocks)
                    {
                        clientsock[numsocks] = accept(server, (struct sockaddr*) &client_addr, &addr_size);
                        if (clientsock[numsocks] == -1)
                            perror("Accept");
                        FD_SET(clientsock[numsocks], &fds);
                        

                        char ip[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
                        printf("Connection established with IP : %s and PORT : %d\n",
                               ip, ntohs(client_addr.sin_port));
                        
                        client_id *client_id = malloc(sizeof(client_id));
                        client_id->socket = clientsock[numsocks];
                        client_id->client_port_no = ntohs(client_addr.sin_port);
                        
                        //printf("Port number: %d\n", ntohs(client_addr.sin_port));
                        char receive_no[20];
                        recv(clientsock[numsocks],receive_no,20,0);

                        //while (1)
                        printf("%s\n",receive_no);
                        int port_no = atoi(receive_no);
                            
                        pthread_create(&message_receive, NULL, socketThread, &port_no);

                        
                        pthread_create(&file_receive, NULL, receive_file, client_id);
                        sleep(1);
                        pthread_create(&file_search, NULL, transfer_file, client_id);
                        numsocks++;
                    }
                    }
                    else
                    {
                        printf("Ran out of socket space.\n");
                    }
                
                pthread_join(message_receive, NULL);
                pthread_join(file_receive, NULL);
                pthread_join(file_search, NULL);

                
                close(i);
                close(server);

                return 0;
            }
        }
    }
}

