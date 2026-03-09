#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
 
int main(int argc, char** argv){ 
    int sockfd, port = atoi(argv[1]); 
    struct sockaddr_in serverAddr, newAddr; 
    socklen_t addr_size = sizeof(newAddr); 
    // Simplified "Frame" variables 
    int frame_id = 0, recv_sq_no, recv_kind, ack_send; 
    char buffer[1024]; 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(port); 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); 
    while(1){ 
        // Instead of receiving a struct, we receive the raw data into our buffer 
        // Note: In a real simplified array version, we treat the first index as ID 
        int f_recv_size = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&newAddr, 
&addr_size);   
        if (f_recv_size > 0) { 
            printf("[+]Frame Received: %s\n", buffer);    
            // Send back the ACK (Sequence number + 1) 
            ack_send = frame_id + 1; 
            sendto(sockfd, &ack_send, sizeof(int), 0, (struct sockaddr*)&newAddr, addr_size); 
            printf("[+]Ack %d Sent\n", ack_send);    
            frame_id++; 
        } 
    } 
    close(sockfd); 
    return 0; 
}