#include <arpa/inet.h> //主要是信息转换 将客户端信息，转换为字符串信息
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // 存放socket相关函数
#include <unistd.h> //unistd.h 中所定义的接口通常都是大量针对系统调用的封装

#define BUF_SIZE 1024

void error_handling(const char* message);

/*
    UDP echo server
*/
int main(int argc, char* argv[])
{
    int serv_sock, client_sock;
    char buff[BUF_SIZE];
    FILE* fp;
    int read_cnt;
    socklen_t clnt_addr_size;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    fp = fopen("file_server.c", "rb");
    // 有了套接字后，需要绑定信息（bind），这样才能进行后续的连接
    serv_sock = socket(PF_INET, // IP protocol family
        SOCK_STREAM, // 无连接
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
    listen(serv_sock, 5);
    clnt_addr_size = sizeof clnt_addr;
    client_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (client_sock == -1) {
        error_handling("accapt() error");
    }
    while (1) {
        //以1字节为单位读取
        read_cnt=fread((void*)buff,1,BUF_SIZE,fp);
        if(read_cnt<BUF_SIZE){
            write(client_sock,buff,read_cnt);
            break;
        }
        write(client_sock,buff,read_cnt);

    }
    shutdown(client_sock,SHUT_WR);
    read(client_sock,buff,BUF_SIZE);
    printf("Message from client: %s \n",buff);
    fclose(fp);
    close(client_sock);
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