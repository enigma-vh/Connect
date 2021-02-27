//
//  registry_function.c
//  Project2
//
//  Created by Van Hao On on 4/14/20.
//  Copyright © 2020 Van Hao On. All rights reserved.
//

#include "registry_function.h"



// function to clear buffer
void clearBuf(char* b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

int exists_test(char* name) {
    struct stat buffer;
    if (stat(name, &buffer) == 0){
        return 1;
    }
    return 0;
}


void *receive_file(void *arg)
{ // Start function
    client_id* client = (client_id*) arg;
    
    int recv_size = 0, size = 0, read_size, write_size, packet_index = 1, stat;
    
    char filearray[10241];
    
    char name_client[100];
    
    sprintf(name_client,"%d.txt",client->client_port_no);
    FILE *file = fopen(name_client, "wb");
    
    //Find the size of the image
    do
    {
        stat = read(client->socket, &size, sizeof(int));
    } while (stat < 0);
    
    printf("Packet received.\n");
    printf("Packet size: %i\n", stat);
    printf("File size: %i\n", size);
    printf(" \n");
    
    char buffer[] = "Got it";
    
    //Send our verification signal
    do
    {
        stat = write(client->socket, &buffer, sizeof(int));
    } while (stat < 0);
    
    printf("Reply sent\n");
    printf(" \n");
    
    if (file == NULL)
    {
        printf("Error has occurred. File could not be opened\n");
        return -1;
    }
    
    //Loop while we have not received the entire file yet
    
    struct timeval timeout = {10, 0};
    
    fd_set fds;
    int buffer_fd;
    
    while (recv_size < size)
    {
        
        FD_ZERO(&fds);
        FD_SET(client->socket, &fds);
        
        buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
        
        if (buffer_fd < 0)
            printf("error: bad file descriptor set.\n");
        
        if (buffer_fd == 0)
            printf("error: buffer read timeout expired.\n");
        
        if (buffer_fd > 0)
        {
            do
            {
                read_size = read(client->socket, filearray, 10241);
            } while (read_size < 0);
            
            printf("Packet number received: %i\n", packet_index);
            printf("Packet size: %i\n", read_size);
            
            //Write the currently read data into our image file
            write_size = fwrite(filearray, 1, read_size, file);
            printf("Written file size: %i\n", write_size);
            
            if (read_size != write_size)
            {
                printf("error in read write\n");
            }
            
            //Increment the total number of bytes read
            recv_size += read_size;
            packet_index++;
            printf("Total received file size: %i\n", recv_size);
            //printf(" \n");
            printf(" \n");
        }
    }
    fclose(file);
    
    FILE* fp = fopen(name_client,"rb");
    
    FILE* append_file = fopen("collect.txt", "a");
    
    char line[600];
    char filename[100];
    char s[500];
    int id = 0;
    
    fprintf(append_file, "%2s  |  %20s    | %10s\n\n","ID","FILE","PORT_NO");
    
    
    while (fgets(line,600,fp) != NULL)
    {
        fscanf(fp, "%2d. %20s", &id, filename);
        sprintf(s, "%2d  |  %20s    | %10d\n\n", id, filename, client->client_port_no);
        fputs(s, append_file);
    }
    
    printf("Image successfully Received!\n");
    fclose(fp);
    fclose(append_file);
}

void* socketThread(void* arg)
{
    int port_no = *((int*)arg);
    
    int server = socket(AF_INET, SOCK_DGRAM, 0);
    if(server < 0){
        printf("Error in server creation!");
    }
    
    struct sockaddr_in server_addr, client_addr;
    
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
    memset(client_addr.sin_zero, '\0', sizeof client_addr.sin_zero);
    

    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);
    
    
    socklen_t addr_size;
    addr_size = sizeof(client_addr);
    char buffer[256], message[256], e_mess[256];
    
    if (bind(server, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    while(1){
        time_t start = time(NULL);
        
        recvfrom(server, buffer, 256, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_size);
        sprintf(message, "Client[%d]: %s -------- %s", port_no, buffer, ctime(&start));
        printf("%s\n",message);
        //sleep(30);
        
        time_t end = time(NULL);
        
        
        double duration = difftime(end, start);
        //printf("Duration: %f\n",duration);
        
        if (duration > 200) {
            printf("Overtime");
        }
    }
    close(server);
    
}

void* transfer_file(void* arg){
    
    client_id* client = (client_id*) arg;
    int socket = client->socket;
    
    char file_name[256];
    
    char client_info[256];
    char line[600];
    char name[100];
    char s[600];
    int count = 0;
    int port_no = 0;
    int result = 0;
    int id = 0;
    
    FILE* fp = fopen("collect.txt","rb");
    
    if( fp == NULL) {
        printf("Error has occurred. File could not be opened\n");
    }
    
    if(recv(socket,file_name,256,0)){
        while (fgets(line, 1000, fp))
        {
            fscanf(fp, "%2d  |  %20s    | %10d", &id, name, &port_no);
            
            if (strcmp(name, file_name) == 0){
                sprintf(s, "%2d  |  %20s    | %10d\n\n", id, name, port_no);
                printf("%s\n", s);
                result = port_no;
                count++;
            }
            
        }
    }
    
    
    
    if(count == 0){
        strcpy(client_info, "File does not exist!");
        send(socket, client_info, 256, 0);
    }
    else{
        sprintf(client_info, "The port number: %d\n", result);
        send(socket, client_info, 256, 0);
    }
    
    fclose(fp);
}
