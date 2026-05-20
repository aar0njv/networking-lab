#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main() {

    int client;
    struct sockaddr_in serverAddr;
    int windowSize = 4;
    int base = 0;
    int nextFrame;
    int ack;


    client = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(client,
            (struct sockaddr*)&serverAddr,
            sizeof(serverAddr));
    printf("Sender Started...\n");

    // Timeout setup
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while(base < 5) {

        // Send window frames
        for(nextFrame = base; nextFrame < base + windowSize && nextFrame < 5; nextFrame++) {

            send(client, &nextFrame, sizeof(nextFrame), 0);
            printf("Sent Frame : %d\n", nextFrame);
        }

        // Receive ACK
        int n = recv(client, &ack, sizeof(ack), 0);

        // Timeout
        if(n < 0) {
            printf("\nTimeout... Go Back To Frame %d\n\n", base);
            continue;
        }

        printf("\nACK %d Received\n\n", ack);

        // Slide window
        base = ack + 1;
        sleep(1);
    }

    close(client);

    return 0;
}