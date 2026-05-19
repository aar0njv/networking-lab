#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <arpa/inet.h>
 
int main() { 
    int client; 
    struct sockaddr_in serverAddr; 
    socklen_t len; 
    char buffer[1024]; 
    
    client = socket(AF_INET, SOCK_DGRAM, 0); 
    
    serverAddr.sin_family = AF_INET;  
    serverAddr.sin_port = htons(8080); 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
 
    while(1) { 

        memset(buffer, 0, sizeof(buffer));
        len = sizeof(serverAddr);
        printf("\nClient (You): "); 
        fgets(buffer, sizeof(buffer), stdin); 
        
        sendto(client, 
            buffer, 
            strlen(buffer), 
            0, 
            (struct sockaddr*)&serverAddr, 
            len);
        if (strncmp(buffer, "exit", 4) == 0) break; 

        memset(buffer, 0, sizeof(buffer));

        len = sizeof(serverAddr);    
     
        recvfrom(client, 
                buffer, 
                sizeof(buffer), 
                0, 
                (struct sockaddr*)&serverAddr,
                &len);

        printf("Server: %s", buffer); 
        if (strncmp(buffer, "exit", 4) == 0) break; 
    } 
    close(client); 
    return 0; 
} 