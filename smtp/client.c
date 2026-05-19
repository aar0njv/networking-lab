#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 

 
int main() { 
    int server, client;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    socklen_t len;

    client = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8080);

    connect(client, 
            (struct sockaddr*)&serverAddr, 
            sizeof(serverAddr));
    
    printf("Connected to SMTP Server...\n");

    while(1) {
        memset(buffer, 0, sizeof(buffer));

        printf("Enter the SMTP Command: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));

        recv(client, buffer, sizeof(buffer), 0);
        printf("Server: %s", buffer);

        if (strncmp(buffer, "221", 3) == 0) {
            break;
        }
    }

    close(client);
    return 0;

}