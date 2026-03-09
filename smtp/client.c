#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#define MAX 100 
 
void exchange(int sd, char *msg, struct sockaddr_in *addr) { 
    char buf[MAX]; 
    sendto(sd, msg, MAX, 0, (struct sockaddr *)addr, sizeof(*addr)); 
    int n = recvfrom(sd, buf, MAX, 0, NULL, NULL); 
    buf[n] = '\0'; 
    printf("S: %s", buf); 
} 
 
int main(int argc, char *argv[]) { 
    int sd; 
    struct sockaddr_in servaddr; 
    char addr_in[MAX], buf[MAX]; 
    sd = socket(AF_INET, SOCK_DGRAM, 0); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[2])); 
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr); 
    // Start Handshake 
    exchange(sd, "Connection Request", &servaddr); 
    exchange(sd, "HELO client.com\n", &servaddr); 
    printf("Enter Sender Email: "); 
    scanf("%s", addr_in); 
    sprintf(buf, "MAIL FROM: <%s>\n", addr_in); 
    exchange(sd, buf, &servaddr); 
    printf("Enter Recipient Email: "); 
    scanf("%s", addr_in); 
    sprintf(buf, "RCPT TO: <%s>\n", addr_in); 
    exchange(sd, buf, &servaddr); 
    exchange(sd, "DATA\n", &servaddr); 
    printf("Enter Mail Body (end with '.' on a new line):\n"); 
    while (1) { 
        fgets(buf, MAX, stdin); 
        sendto(sd, buf, MAX, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)); 
        if (strncmp(buf, ".", 1) == 0) break; 
    } 
    exchange(sd, "QUIT\n", &servaddr); 
    close(sd); 
    return 0; 
}