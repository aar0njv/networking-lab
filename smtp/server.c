#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#define MAX 100 
 
int main(int argc, char *argv[]) { 
    int sock_fd, n; 
    struct sockaddr_in servaddr, cliaddr; 
    char buffer[MAX]; 
    socklen_t len = sizeof(cliaddr); 
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);     
    memset(&servaddr, 0, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[1])); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    bind(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)); 
    printf("SMTP Server Waiting on Port %s...\n", argv[1]); 
    // 1. Connection Request 
    recvfrom(sock_fd, buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len); 
    sendto(sock_fd, "220 Welcome to Mail Server\n", MAX, 0, (struct sockaddr *)&cliaddr, 
len); 
 
    // 2. HELO 
    n = recvfrom(sock_fd, buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len); 
    buffer[n] = '\0'; printf("C: %s", buffer); 
    sendto(sock_fd, "250 Hello Client\n", MAX, 0, (struct sockaddr *)&cliaddr, len); 
    // 3. MAIL FROM 
    n = recvfrom(sock_fd, buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len); 
    buffer[n] = '\0'; printf("C: %s", buffer); 
    sendto(sock_fd, "250 Sender OK\n", MAX, 0, (struct sockaddr *)&cliaddr, len); 
    // 4. RCPT TO 
    n = recvfrom(sock_fd, buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len); 
    buffer[n] = '\0'; printf("C: %s", buffer); 
    sendto(sock_fd, "250 Recipient OK\n", MAX, 0, (struct sockaddr *)&cliaddr, len); 
    // 5. DATA 
    n = recvfrom(sock_fd, buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len); 
    sendto(sock_fd, "354 Start mail input; end with <CRLF>.<CRLF>\n", MAX, 0, (struct 
sockaddr *)&cliaddr, len); 
 
    // 6. Receive Mail Content 
    while (1) { 
        n = recvfrom(sock_fd, buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len); 
        buffer[n] = '\0'; 
        printf("Mail Data: %s", buffer); 
        if (strncmp(buffer, ".", 1) == 0) break; 
    } 
    // 7. QUIT 
    sendto(sock_fd, "221 Service closing transmission channel\n", MAX, 0, (struct sockaddr 
*)&cliaddr, len);   
    close(sock_fd); 
    return 0; 
}