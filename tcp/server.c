#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<stdlib.h> 
#include<netinet/in.h> 
#include<unistd.h> 
 
int main() { 
    int serversocket, clientsocket, port; 
    struct sockaddr_in serveraddr, clientaddr; 
    socklen_t len; 
    char message[100]; 
    serversocket = socket(AF_INET, SOCK_STREAM, 0); 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = INADDR_ANY; 
    printf("Enter the port number: "); 
    scanf("%d", &port); 
    getchar(); // Consume the newline left by scanf 
    serveraddr.sin_port = htons(port); 
    bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 

    listen(serversocket, 5); 
    printf("\nWaiting for client connection...\n"); 
    len = sizeof(clientaddr); 
    clientsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len); 
    printf("Client connected!\n"); 
 
    // --- CHAT LOOP START --- 
    while(1) { 
        bzero(message, 100); 
        read(clientsocket, message, sizeof(message)); 
        printf("\nClient: %s", message); 
        if (strncmp(message, "exit", 4) == 0) { 
            printf("Client closed the chat.\n"); 
            break; 
        } 
        printf("Server (You): "); 
        fgets(message, 100, stdin); 
        write(clientsocket, message, sizeof(message)); 
        if (strncmp(message, "exit", 4) == 0) break; 
    } 
    // --- CHAT LOOP END --- 

    close(clientsocket); 
    close(serversocket); 
    return 0; 
} 