#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<stdlib.h> 
#include<netinet/in.h> 
#include<unistd.h> 
 
int main() { 
    int server, client;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len;
    char buffer[1024];

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0) {
        printf("Socket connection failed\n");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(server,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr));
    
    listen(server, 5);

    printf("Waiting for client connections...\n");

    len = sizeof(clientAddr);

    client = accept(server,
                    (struct sockaddr*)&clientAddr,
                    &len);

    if (client < 0){
        printf("Accept failed.\n");
        exit(1);
    }
    printf("Client connected successfully...\n");

    while(1) { 
        
        memset(buffer, 0, sizeof(buffer));

        recv(client, buffer, sizeof(buffer), 0);
        printf("\nClient: %s", buffer); 
        if (strncmp(buffer, "exit", 4) == 0) { 
            printf("Client closed the chat.\n"); 
            break; 
        } 
        printf("Server (You): "); 
        fgets(buffer, sizeof(buffer), stdin); 
        send(client, buffer, strlen(buffer), 0); 
        if (strncmp(buffer, "exit", 4) == 0) break; 
    } 
    // --- CHAT LOOP END --- 

    close(client); 
    close(server); 
    return 0; 
} 