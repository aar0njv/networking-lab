#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
 
int main() { 
    int server, client;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len;
    char buffer[1024];
 
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(server, 
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr));
    
    listen(server, 5);
    printf("SMTP Server waiting for Client...\n");

    len = sizeof(clientAddr);

    client = accept(server,
                    (struct sockaddr*)&clientAddr,
                    &len);
    
    printf("Client connected Successfully...\n");

    while(1) {
        memset(buffer, 0, sizeof(buffer));

        //SMTP Command
        recv(client, buffer, sizeof(buffer), 0);
        printf("Client: %s", buffer);

        //Quit condition
        if (strncmp(buffer, "QUIT", 4) == 0) {
            strcpy(buffer, "221 Connection Closed.\n");
            send(client, buffer, strlen(buffer), 0);
            break;
        }

        if(strncmp(buffer, "HELO", 4) == 0) {
            strcpy(buffer, "250 Hello CLient.\n");
        }
        else if(strncmp(buffer, "MAIL FROM", 10) == 0) {
            strcpy(buffer, "250 Sender OK.\n");
        }
        else if(strncmp(buffer, "RCPT TO", 7) == 0) {
            strcpy(buffer, "250 Recipient OK.\n");
        }
        else if(strncmp(buffer, "DATA", 4) == 0) {
            strcpy(buffer, "354 Enter Message: \n");
        }
        else {
            strcpy(buffer, "250 Message Accepted.\n");
        }

        send(client, buffer, strlen(buffer), 0);

    }
    
    
    close(server);
    close(client);

    return 0; 
}