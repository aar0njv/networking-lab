#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#define SERV_TCP_PORT 5035 
#define MAX 60 
 
int main() 
{ 
    int sockfd, n; 
    struct sockaddr_in serv_addr; 
    char sendline[MAX], recvline[MAX]; 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    serv_addr.sin_port = htons(SERV_TCP_PORT); 
    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    printf("\nEnter the source file name: "); 
    scanf("%s", sendline); 
    // Send filename to server 
    write(sockfd, sendline, MAX); 
    printf("\n--- File Content ---\n"); 
    while ((n = read(sockfd, recvline, MAX)) > 0) { 
        recvline[n] = '\0'; // Ensure string termination 
        printf("%s", recvline); 
    } 
    close(sockfd); 
    return 0; 
}