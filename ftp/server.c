#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main() {

    int server, client;

    struct sockaddr_in serverAddr, clientAddr;

    socklen_t len;

    char buffer[1024];

    FILE *fp;

    // Create socket
    server = socket(AF_INET, SOCK_STREAM, 0);

    if(server < 0) {
        printf("Socket creation failed");
        exit(1);
    }

    // Server configuration
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Bind socket
    bind(server,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));

    // Listen
    listen(server, 5);

    printf("FTP Server Waiting...\n");

    len = sizeof(clientAddr);

    // Accept client
    client = accept(server,
                    (struct sockaddr*)&clientAddr,
                    &len);

    if(client < 0) {
        printf("Accept failed");
        exit(1);
    }

    printf("Client connected successfully...\n");

    // Create new file in server directory
    fp = fopen("received.txt", "w");

    if(fp == NULL) {
        printf("File creation failed");
        exit(1);
    }

    // Receive file data
    while(1) {

        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(client,
                         buffer,
                         sizeof(buffer),
                         0);

        if(bytes <= 0) {
            break;
        }

        // Write data into file
        fprintf(fp, "%s", buffer);
    }

    printf("File received successfully.\n");

    // Close file
    fclose(fp);

    // Close sockets
    close(client);
    close(server);

    return 0;
}