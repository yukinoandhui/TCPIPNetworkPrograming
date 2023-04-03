#include <arpa/inet.h> //主要是信息转换 将客户端信息，转换为字符串信息
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // 存放socket相关函数
#include <unistd.h> //unistd.h 中所定义的接口通常都是大量针对系统调用的封装
#define OPSZ 4
#define BUF_SIZE 1024
int calculate(int opnum, int opnds[], char op);
void error_handling(const char* message);
int main(int argc, char* argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char opinfo[BUF_SIZE];
    int reslut, opnd_cnt, i;
    int recv_cnt, recv_len;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    // 有了套接字后，需要绑定信息（bind），这样才能进行后续的连接
    serv_sock = socket(PF_INET, // IP protocol family
        SOCK_STREAM,
        0); // PF_INET指明通信域，SOCK_STREAM（面向连接可靠方式）
    if (serv_sock == -1) {
        error_handling("socker() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // htonl 无符号长整形 本机字节顺序转换为网络字节 即大端模式(big-endian)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 无符号短整型数值转换为网络字节序
    serv_addr.sin_port = htons(atoi(argv[1]));
    // 分配ip地址和端口号
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    // 绑定信息后，准备连接，也就是进入可接收请求状态
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    clnt_addr_size = sizeof(clnt_addr);
    // 收到消息后，accept接收消息
    for (i = 0; i < 5; i++) {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // 三次握手发生在accept之前，等待队列里就是已经握手的
        if (clnt_sock == -1) {
            error_handling("accapt() error");
        } else {
            printf("Connected client %d\n", i + 1);
        }
        read(clnt_sock, &opnd_cnt, 1);
        recv_len = 0;
        while ((opnd_cnt * OPSZ + 1) > recv_len) {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
            recv_len += recv_cnt;
        }
        reslut = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
        write(clnt_sock, (char*)&reslut, sizeof reslut);
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}
void error_handling(const char* message)
{
    // fputs指定向哪个流输出信息，此外puts会自动换行，fputs不会
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
int calculate(int opnum, int opnds[], char op)
{
    int result = opnds[0], i;
    switch (op) {
    case '+':
        for (i = 1; i < opnum; i++) {
            result += opnds[i];
        }
        break;
    case '-':
        for (i = 1; i < opnum; i++) {
            result -= opnds[i];
        }
        break;
    case '*':
        for (i = 1; i < opnum; i++) {
            result *= opnds[i];
        }
        break;
    }
    return result;
}