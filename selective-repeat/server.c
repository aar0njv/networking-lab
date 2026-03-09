#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/time.h> 
 
void resend(int msg_id, int sock) { 
    char buf[50]; 
    sprintf(buf, "server message :%d", msg_id); 
    printf("Resending Message %d\n", msg_id); 
    write(sock, buf, sizeof(buf)); 
} 
 
int main() { 
    int s_sock, c_sock, tot = 0; 
    struct sockaddr_in server, client; 
    socklen_t len = sizeof(client); 
    char buff[50]; 
    s_sock = socket(AF_INET, SOCK_STREAM, 0); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(9009); 
    server.sin_addr.s_addr = INADDR_ANY; 
    bind(s_sock, (struct sockaddr *)&server, sizeof(server)); 
    listen(s_sock, 5); 
    c_sock = accept(s_sock, (struct sockaddr *)&client, &len); 
 
    while (tot < 9) { 
        // Send Window of 3 
        for (int i = tot; i < tot + 3 && i < 9; i++) { 
            sprintf(buff, "server message :%d", i); 
            write(c_sock, buff, sizeof(buff)); 
            usleep(1000); 
        } 
        // Wait for 3 individual ACKs 
        for (int k = tot; k < tot + 3 && k < 9; k++) { 
        wait_ack: 
            fd_set set; 
            struct timeval timeout = {2, 0}; 
            FD_ZERO(&set); FD_SET(c_sock, &set); 
            if (select(c_sock + 1, &set, NULL, NULL, &timeout) == 0) { 
                resend(k, c_sock); // Timeout 
                goto wait_ack; 
            } else { 
                read(c_sock, buff, sizeof(buff)); 
                if (buff[0] == 'n') { // Negative ACK 
                    resend(buff[strlen(buff) - 1] - '0', c_sock); 
                    goto wait_ack; 
                } else tot++; 
            } 
        } 
    } 
    close(c_sock); close(s_sock); 
    return 0; 
}