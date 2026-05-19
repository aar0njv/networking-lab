#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h>
#include<arpa/inet.h>
 
int main() { 
    int client; 
    struct sockaddr_in serverAddr; 
    char buffer[1024]; 

    client = socket(AF_INET, SOCK_STREAM, 0); 
    
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client, 
            (struct sockaddr*)&serverAddr,
            sizeof(serverAddr)); 
    printf("Connected to server!\n"); 
 
    while(1) { 
        memset(buffer, 0, sizeof(buffer));

        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);

        send(client, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        recv(client, buffer, sizeof(buffer), 0);

        printf("\nServer: %s", buffer);

        if(strncmp(buffer, "exit", 4) == 0) {
            printf("Server closed the chat.\n");
            break;
        }

    } 

    close(client); 
    return 0; 
}