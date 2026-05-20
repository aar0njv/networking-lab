#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
 
int main(int argc, char** argv){ 
    
    int server, client;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len;
    int frame, ack;
    int expected = 0;

    srand(time(NULL));

    server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    listen(server, 5);
    printf("Server waiting for connection...\n");

    len = sizeof(clientAddr);

    client = accept(server, 
                    (struct sockaddr*)&clientAddr,
                    &len);
    printf("Connection Established...\n\n");
    

    while(1) {

        recv(client, &frame, sizeof(frame), 0);

        if (rand() % 10 < 2) {
            printf("ACK Lost \n\n");
            continue;
        }

        if (frame == expected) {    // Correct Frame
            printf("Recievec Frame: %d\n", frame);
            ack = frame;
            expected++;
        }

        else {  // Duplicate frame
            printf("Duplicate Frame: %d\n", frame);
            ack = expected - 1;
        }

        send(client, &ack, sizeof(ack), 0);

        printf("ACK %d Sent\n\n", ack);
    }

    close(client);
    close(server);

    return 0;
}