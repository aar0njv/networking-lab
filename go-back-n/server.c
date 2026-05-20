#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>

int main() {

    int server, client;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len;
    int frame, ack;
    int expected = 0;

    srand(time(NULL));

    // Create socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(server,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));

    listen(server, 5);

    printf("Receiver Waiting...\n");

    len = sizeof(clientAddr);

    // Accept connection
    client = accept(server,
                    (struct sockaddr*)&clientAddr,
                    &len);

    printf("Connection Established...\n");


    while(1) {

        recv(client, &frame, sizeof(frame), 0);

        // Random frame loss
        if(rand() % 10 < 2) {
            printf("Frame %d Lost\n\n", frame);
            continue;
        }

        // Correct frame
        if(frame == expected) {
            printf("Received Frame : %d\n", frame);
            ack = frame;
            expected++;
        }

        // Out-of-order frame
        else {
            printf("Discarded Frame : %d\n", frame);
            ack = expected - 1;
        }

        // Send ACK
        send(client, &ack, sizeof(ack), 0);
        printf("ACK %d Sent\n\n", ack);
    }

    close(client);
    close(server);

    return 0;
}