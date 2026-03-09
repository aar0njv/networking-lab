#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
 
int main() { 
    int clientsocket, port; 
    struct sockaddr_in serveraddr; 
    socklen_t len; 
    char message[100]; 
    clientsocket = socket(AF_INET, SOCK_DGRAM, 0); 
    serveraddr.sin_family = AF_INET; 
    printf("Enter Port: "); 
    scanf("%d", &port); 
    getchar(); 
    serveraddr.sin_port = htons(port); 
    serveraddr.sin_addr.s_addr = INADDR_ANY; 
 
    while(1) { 
        printf("\nClient (You): "); 
        fgets(message, 100, stdin); 
         
        // Send to server 
        sendto(clientsocket, message, sizeof(message), 0, (struct sockaddr*)&serveraddr, 
sizeof(serveraddr)); 
        if (strncmp(message, "exit", 4) == 0) break; 
        bzero(message, 100); 
        len = sizeof(serveraddr);    
        // Receive reply 
        recvfrom(clientsocket, message, sizeof(message), 0, (struct sockaddr*)&serveraddr, 
&len); 
        printf("Server: %s", message); 
        if (strncmp(message, "exit", 4) == 0) break; 
    } 
    close(clientsocket); 
    return 0; 
} 