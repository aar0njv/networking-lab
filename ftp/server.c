#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h> 
 
#define SERV_TCP_PORT 5035 
#define MAX 60 
 
int main() 
{ 
    int sockfd, newsockfd, clength; 
    struct sockaddr_in serv_addr, cli_addr; 
    char str[MAX], buff[4096]; 
    FILE *f1; 
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(SERV_TCP_PORT); 
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    listen(sockfd, 5); 
    printf("\nServer Listening..."); 
    clength = sizeof(cli_addr); 
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clength);     
    // Read filename from client 
    read(newsockfd, str, MAX); 
    printf("\nClient requested file: %s\n", str); 
    f1 = fopen(str, "r"); 
    if (f1 != NULL) { 
        while (fgets(buff, sizeof(buff), f1) != NULL) { 
            write(newsockfd, buff, MAX); 
        } 
        fclose(f1); 
    } 
    printf("\nTransfer complete.\n"); 
    close(newsockfd); 
    close(sockfd); 
    return 0; 
}