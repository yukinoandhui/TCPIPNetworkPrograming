#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUF_SIZE 1024
void error_handling(const char* message);

/*

*/
int main(int argc, char* argv[])
{
    int tcp_sock,udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen=sizeof sock_type;
    tcp_sock=socket(PF_INET,SOCK_STREAM,0);
    udp_sock=socket(PF_INET,SOCK_DGRAM,0);
    printf("SOCK_STREAM: %d\n",SOCK_STREAM);
    printf("SOCK_DGRAM: %d\n",SOCK_DGRAM);
    //TCP
    state=getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
    if(state){
        error_handling("getsockopt() error");
    }
    printf("Socket type one: %d\n",sock_type);

    state=getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
    if(state){
        error_handling("getsockopt() error");
    }
    printf("Socket type two: %d\n",sock_type);

    //UDP   
    return 0;
}
void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}