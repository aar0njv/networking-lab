#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
 
int main() { 
    int c_sock; 
    struct sockaddr_in server; 
    char buff[100], ack[50]; 
    int expected_frame = 0; 
    int simulate_loss = 1; // Used to drop frame 8 once 
    c_sock = socket(AF_INET, SOCK_STREAM, 0); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(9009); 
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    connect(c_sock, (struct sockaddr *)&server, sizeof(server)); 
    printf("Connected to Server...\n"); 
 
    while (expected_frame <= 9) { 
        read(c_sock, buff, sizeof(buff)); 
        int received_id = buff[strlen(buff) - 1] - '0'; 
 
        // Simulate a loss for frame 8 
        if (received_id == 8 && simulate_loss) { 
            printf("Simulating loss of Frame 8...\n"); 
            simulate_loss = 0; 
            continue; // Don't send ACK, don't increment expected_frame 
        } 
        if (received_id == expected_frame) { 
            printf("Received Frame %d. Sending ACK.\n", received_id); 
            sprintf(ack, "ack-%d", received_id); 
            write(c_sock, ack, sizeof(ack)); 
            expected_frame++; 
        } else { 
            printf("Discarded Frame %d (Expected %d)\n", received_id, expected_frame); 
            // In GBN, we don't ACK out-of-order frames 
        } 
    } 
    close(c_sock); 
    return 0; 
} 
