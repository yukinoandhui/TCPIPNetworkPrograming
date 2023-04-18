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
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    int read_cnt;
    socklen_t adr_sz;
    FILE* fp;
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
    }
    fp = fopen("receive.dat", "wb");
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    connect(sock, (struct sockaddr*)&serv_addr, sizeof serv_addr);

    while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0) {
        fwrite((void*)buf, 1, read_cnt, fp);
    }
    printf("Received file data\n");
    write(sock, "Thank you", 10);
    fclose(fp);
    close(sock);
    return 0;
}
void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}