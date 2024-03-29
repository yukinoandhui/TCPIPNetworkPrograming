#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUF_SIZE 1024
void error_handling(const char* message);

/*
    UDP echo client
*/
int main(int argc, char* argv[])
{

    int sock;
    struct sockaddr_in serv_addr,from_adr;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
    }
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
   
    
    while (1) {
        fputs("Input message(q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        /*
            这里可以用connect+write来代替sendto
        */
        sendto(sock,message,strlen(message),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        adr_sz=sizeof from_adr;
        str_len=recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&from_adr,&adr_sz);
        message[str_len]=0;
        
        
        
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}
void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}