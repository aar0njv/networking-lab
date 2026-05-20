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
    int frames[10];
    int ack;
    int received[10] = {0};
    int i, j;

    client = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client,
            (struct sockaddr*)&serverAddr,
            sizeof(serverAddr));
    printf("Sender Started...\n");

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    for(i = 0; i < 10; i++) {
        frames[i] = i;
    }
    int base = 0;

    while(base < 10) {

        // Send window frames
        for(i = base; i < base + windowSize && i < 10; i++) {

            // Send only unacknowledged frames
            if(received[i] == 0) {
                send(client, &frames[i], sizeof(frames[i]), 0);
                printf("Sent Frame : %d\n", frames[i]);
            }
        }

        // Receive ACKs
        for(j = base; j < base + windowSize && j < 10; j++) {
            int n = recv(client, &ack, sizeof(ack), 0);
            if(n < 0) {
                printf("Timeout for Frame %d\n", j);
            }
            else {
                printf("ACK %d Received\n", ack);
                received[ack] = 1;
            }
        }

        // Slide window
        while(received[base] == 1) {
            base++;
        }
        printf("\n");
    }

    close(client);
    return 0;
}