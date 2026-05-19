#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
 
int main() { 
    int server; 
    struct sockaddr_in serverAddr, clientAddr; 
    socklen_t len; 
    char buffer[1024]; 

    server = socket(AF_INET, SOCK_DGRAM, 0); 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY;  
    serverAddr.sin_port = htons(8080); 

    bind(server, 
        (struct sockaddr*)&serverAddr, 
        sizeof(serverAddr)); 
    
        printf("UDP Server Waiting...\n"); 
        
    len = sizeof(clientAddr); 
    
    while(1) { 
        memset(buffer, 0, sizeof(buffer));

        recvfrom(server, 
            buffer, 
            sizeof(buffer), 
            0, 
            (struct sockaddr*)&clientAddr, 
            &len); 
        
        printf("\nClient: %s", buffer); 
        if (strncmp(buffer, "exit", 4) == 0) break; 
        
        
        printf("Server (You): "); 
        fgets(buffer, sizeof(buffer), stdin);   
        sendto(server, 
            buffer, 
            strlen(buffer), 
            0, 
            (struct sockaddr*)&clientAddr, 
            len); 
        if (strncmp(buffer, "exit", 4) == 0) break; 
    
    } 
    close(server); 
    return 0; 
}