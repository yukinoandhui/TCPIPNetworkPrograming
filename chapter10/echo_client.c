#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUF_SIZE 1024
void error_handling(const char* message);
int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1) {
        error_handling("connect() error!");
    }
    puts("Connected......");
    while (1) {
        fputs("Input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        /**
         * 由于客户端的消息会被服务器端放在缓冲区中，所以可能客户端发送了好几条消息，服务器一次性取走完。
         * 简单来说就是数据会不会自动分割，比如两个结构体连续存在一段内存中，那是有边界的，结构体把其分割了。
         * 若把其数据拷贝到数组里面，那是无边界的，因为分不清从哪里才是分割线。
         * 所以可以根据echo的特性来解决：我们可以提前知道一次性
         *
         */
        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        while (recv_len < str_len) {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if (recv_cnt == -1) {
                error_handling("read() error!");
            }
            recv_len += recv_cnt;
        }
        message[recv_len]=0;
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