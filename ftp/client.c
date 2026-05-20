#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main() {

    int client;

    struct sockaddr_in serverAddr;

    char buffer[1024];

    char filename[100];

    FILE *fp;

    // Create socket
    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0) {
        printf("Socket creation failed");
        exit(1);
    }

    // Server configuration
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(AF_INET,
              "127.0.0.1",
              &serverAddr.sin_addr);

    // Connect to server
    if(connect(client,
              (struct sockaddr*)&serverAddr,
              sizeof(serverAddr)) < 0) {

        printf("Connection failed");
        exit(1);
    }

    printf("Connected to FTP Server...\n");

    // Enter filename
    printf("Enter filename: ");

    scanf("%s", filename);

    // Open file
    fp = fopen(filename, "r");

    if(fp == NULL) {
        printf("File not found");
        exit(1);
    }

    // Read and send file data
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {

        send(client,
             buffer,
             strlen(buffer),
             0);
    }

    printf("File sent successfully.\n");

    // Close file
    fclose(fp);

    // Close socket
    close(client);

    return 0;
}