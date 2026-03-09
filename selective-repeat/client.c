#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include <time.h> 
#include <unistd.h> 
 
int main() { 
    srand(time(0)); 
    int c_sock = socket(AF_INET, SOCK_STREAM, 0); 
    struct sockaddr_in serv = {AF_INET, htons(9009), inet_addr("127.0.0.1")}; 
    connect(c_sock, (struct sockaddr*)&serv, sizeof(serv)); 
    char buff[100], reply[50]; 
    int count = 0; 
 
    while (count < 9) { 
        read(c_sock, buff, sizeof(buff)); 
        int id = buff[strlen(buff)-1] - '0'; 
        printf("Received: %s", buff); 
        if (rand() % 4 == 0) { // 25% chance of corruption 
            printf(" [Corrupted]\n"); 
            sprintf(reply, "negative akwn-%d", id); 
        } else { 
            printf(" [OK]\n"); 
            sprintf(reply, "akwnowledgementof-%d", id); 
            count++; 
        } 
        write(c_sock, reply, sizeof(reply)); 
    } 
    close(c_sock); 
    return 0; 
} 
