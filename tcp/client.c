#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
 
int main() { 
    int clientsocket, port; 
    struct sockaddr_in serveraddr; 
    char message[100]; 
    clientsocket = socket(AF_INET, SOCK_STREAM, 0); 
    serveraddr.sin_family = AF_INET; 
    printf("Enter the port number: "); 
    scanf("%d", &port); 
    getchar(); // Consume the newline 
    serveraddr.sin_port = htons(port); 
    connect(clientsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 
    printf("Connected to server!\n"); 
 
    // --- CHAT LOOP START --- 
    while(1) { 
        printf("\nClient (You): "); 
        fgets(message, 100, stdin); 
        write(clientsocket, message, sizeof(message)); 
        if (strncmp(message, "exit", 4) == 0) break; 
        bzero(message, 100); 
        read(clientsocket, message, sizeof(message)); 
        printf("Server: %s", message); 
        if (strncmp(message, "exit", 4) == 0) { 
            printf("Server closed the chat.\n"); 
            break; 
        } 
    } 
    // --- CHAT LOOP END --- 
    close(clientsocket); 
    return 0; 
}