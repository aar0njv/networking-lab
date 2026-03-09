#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h> 
 
int main(int argc, char *argv[]){ 
    int sockfd, port = atoi(argv[1]); 
    struct sockaddr_in serverAddr; 
    socklen_t addr_size = sizeof(serverAddr); 
    int frame_id = 0, ack_recv_val; 
    char buffer[1024]; 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(port); 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
     
    while(1){ 
        printf("Enter Data: "); 
        scanf("%s", buffer); 
        // Send raw buffer (The "Frame") 
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&serverAddr, 
sizeof(serverAddr)); 
        printf("[+]Frame %d Sent\n", frame_id); 
        // Wait for ACK (just an integer) 
        int n = recvfrom(sockfd, &ack_recv_val, sizeof(int), 0, NULL, NULL);      
        if(n > 0 && ack_recv_val == frame_id + 1){ 
            printf("[+]Ack Received: %d\n", ack_recv_val); 
            frame_id++; 
        } else { 
            printf("[-]Ack Error. Resending...\n"); 
            // In a real logic, you wouldn't increment frame_id here 
        } 
    } 
    close(sockfd); 
    return 0; 
}