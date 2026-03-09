#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
 
int main() { 
    int serversocket, port; 
    struct sockaddr_in serveraddr, clientaddr; 
    socklen_t len; 
    char message[100]; 
    // Note: SOCK_DGRAM is used for UDP 
    serversocket = socket(AF_INET, SOCK_DGRAM, 0); 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = INADDR_ANY;  
    printf("Enter Port: "); 
    scanf("%d", &port); 
    getchar(); 
    serveraddr.sin_port = htons(port); 
    bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 
    printf("UDP Server Waiting...\n"); 
 
    while(1) { 
        len = sizeof(clientaddr); 
        bzero(message, 100); 
        // Receive from anyone 
        recvfrom(serversocket, message, sizeof(message), 0, (struct sockaddr*)&clientaddr, 
&len); 
        printf("\nClient: %s", message); 
        if (strncmp(message, "exit", 4) == 0) break; 
        printf("Server (You): "); 
        fgets(message, 100, stdin);   
        // Send back to the specific client that just messaged us 
        sendto(serversocket, message, sizeof(message), 0, (struct sockaddr*)&clientaddr, len); 
        if (strncmp(message, "exit", 4) == 0) break; 
    } 
    close(serversocket); 
    return 0; 
}