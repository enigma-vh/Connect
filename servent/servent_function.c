//
//  servent_function.c
//  Project2
//
//  Created by Van Hao On on 4/14/20.
//  Copyright © 2020 Van Hao On. All rights reserved.
//

#include "servent_function.h"


void clearBuf(char* b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}


//directory file

void directory(void* arg)
{
    FILE* file = fopen("filenames.txt","w");
    int socket = *((int *)arg);
    
    struct dirent *de; // Pointer for directory entry
    DIR *dr = opendir(".");
    int i =0;
    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
    }
    while ((de = readdir(dr)) != NULL){
        fprintf(file,("%d. %s\n"), i, de->d_name);
        i++;
    }
    fclose(file);
    closedir(dr);
    //file = fopen("filenames.txt", "rb");
}


// "publish"

void* publish(void* arg)
{
    int size, read_size, stat, packet_index;
    char send_buffer[10240], read_buffer[NET_BUF_SIZE];
    
    int socket = *((int *)arg);
    packet_index = 1;
    
    directory(arg);
    FILE* file = fopen("filenames.txt", "rb");
    
    printf("Getting Picture Size\n");
    
    if (file == NULL)
    {
        printf("Error Opening Image File");
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    printf("Total Picture size: %i\n", size);
    
    //Send Picture Size
    printf("Sending Picture Size\n");
    write(socket, (void *)&size, sizeof(int));
    
    //Send Picture as Byte Array
    printf("Sending Picture as Byte Array\n");
    
    do
    { //Read while we get errors that are due to signals.
        stat = read(socket, &read_buffer, NET_BUF_SIZE);
        printf("Bytes read: %i\n", stat);
    }while (stat < 0);
    
    printf("Received data in socket\n");
    //printf("Socket data: %c\n", read_buffer);
    
    while (!feof(file))
    {

        //Read from the file into our send buffer
        read_size = fread(send_buffer, 1, sizeof(send_buffer) - 1, file);
        
        //Send data through our socket
        do
        {
            stat = write(socket, send_buffer, read_size);
        } while (stat < 0);
        
        printf("Packet Number: %i\n", packet_index);
        printf("Packet Size Sent: %i\n", read_size);
        printf(" \n");
        printf(" \n");
        
        packet_index++;
        
        //Zero out our send buffer
        bzero(send_buffer, sizeof(send_buffer));
    }
    fclose(file);
}


void* send_message(void* arg){
    int port_no = *((int*)arg);
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[256], e_mess[256];
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    
    server.sin_family = AF_INET;
    server.sin_port = htons(port_no);
    server.sin_addr.s_addr = INADDR_ANY;
    
    
    strcpy(buffer,"Hello");

    while (1){
        strcpy(buffer, "Hello");
        
        if (sendto(sockfd, buffer, 256, 0, (const struct sockaddr *)&server, sizeof(server)) == -1)
        {
            printf("Error!\n");
        }
        else
        {
            sleep(30);
        }
    }
    
    close(sockfd);
}

void* search_file(void* arg){
    int socket = *((int*)arg);
    char choice[2];
    char file_name[256], client_id[256];

    while(strcmp(choice, "S") != 0 && strcmp(choice, "s") != 0){
        printf("\nPress [S] to start searching -------- ");
        scanf("%s", choice);
    }
    
    fflush(stdin);
    printf("File name: ");
    scanf("%s", file_name);
    send(socket, file_name, 256, 0);
    recv(socket, client_id, 256, 0);
    printf("%s\n", client_id);
}







