#define _GNU_SOURCE 
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>


#define BUF_SIZE 30
void error_handling(const char* message);
int main(int argc, char* argv[])
{
    int recv_sock,str_len;
    struct sockaddr_in adr;
    struct ip_mreq join_adr;

    char buf[BUF_SIZE];
    if (argc != 3) {
        printf("Usage : %s <GroupIP> <port>\n", argv[0]);
        exit(1);
    }
    recv_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&adr, 0, sizeof adr);
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[2]));

    join_adr.imr_multiaddr.s_addr=inet_addr(argv[1]);
    join_adr.imr_interface.s_addr=htonl(INADDR_ANY);

    setsockopt(recv_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(void*)&join_adr,sizeof join_adr);
    while (1)
    {
        //通过调用recvfrom函数接收多播数据。如果不需要知道传输数据的主机地址信息，可以向recvfrom函数的第五个和第六个参数分别传递NULL和0。
        puts("?");
        str_len=recvfrom(recv_sock,buf,BUF_SIZE-1,0,NULL,0);
        if(str_len<0){
            break;
        }
        if (str_len==0)
        {
            puts("收到0个");
        }
        buf[str_len]=0;
        puts("收到了");
        fputs(buf,stdout);
    }
    close(recv_sock);
    return 0;
}
void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}