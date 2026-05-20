#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/time.h>

int main() {

    int client;
    struct sockaddr_in serverAddr;
    int frame = 0, ack;

    client = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(client,
            (struct sockaddr*)&serverAddr,
            sizeof(serverAddr));

    printf("Connected to server...\n\n");

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while(frame < 5) {
        send(client, &frame, sizeof(frame), 0);
        printf("Sent Frame %d\n", frame);

        int n = recv(client, &ack, sizeof(ack), 0);
        if(n < 0){
            printf("Timeout... Resending Frame %d.\n\n", frame);
            continue;
        }

        if (ack == frame) {
            printf("ACK %d Recieved.\n\n", ack);
            frame++;
        }
        sleep(1);
    }

    close(client);

    return 0;
}