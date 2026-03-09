#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <sys/time.h> 
#include <stdlib.h> 
 
int main() { 
    int s_sock, c_sock; 
    struct sockaddr_in server, client; 
    socklen_t add = sizeof(client); 
    char buff[50]; 
    int next_to_send = 0; // The frame we are currently sending 
    int window_size = 3; 
 
    s_sock = socket(AF_INET, SOCK_STREAM, 0); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(9009); 
    server.sin_addr.s_addr = INADDR_ANY; 
    bind(s_sock, (struct sockaddr *)&server, sizeof(server)); 
    listen(s_sock, 5); 
    printf("Server Up - Go-Back-N (N=3)\n"); 
    c_sock = accept(s_sock, (struct sockaddr *)&client, &add); 
 
    while (next_to_send <= 9) { 
        // Send a Window of 3 frames 
        for (int i = 0; i < window_size && (next_to_send + i) <= 9; i++) { 
            sprintf(buff, "server message :%d", next_to_send + i); 
            printf("Sending Frame %d\n", next_to_send + i); 
            write(c_sock, buff, sizeof(buff)); 
            usleep(1000); 
        } 
        // Wait for ACK of the FIRST frame in the window 
        fd_set set; 
        struct timeval timeout = {2, 0}; // 2 second timer 
        FD_ZERO(&set); 
        FD_SET(c_sock, &set); 
        int rv = select(c_sock + 1, &set, NULL, NULL, &timeout); 
        if (rv == 0) { // Timeout occurred 
            printf("Timeout for Frame %d! Going back...\n", next_to_send); 
            // Logic: We don't increment next_to_send, so the loop restarts and resends 
        } else { 
            read(c_sock, buff, sizeof(buff)); 
            printf("Received ACK: %s\n", buff); 
            next_to_send++; // Slide the window forward by 1 
        } 
    } 
 
    printf("All frames sent successfully.\n"); 
    close(c_sock); close(s_sock); 
    return 0;