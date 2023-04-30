#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(const char* message);
int main(int argc, char* argv[])
{
    int send_sock;
    struct sockaddr_in mult_adr;
    int time_live=TTL;
    FILE*fp;
    char buf[BUF_SIZE];
    if (argc != 3) {
        printf("Usage : %s <GroupIP> <port>\n", argv[0]);
        exit(1);
    }
    send_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&mult_adr, 0, sizeof mult_adr);
    mult_adr.sin_family = AF_INET;
    mult_adr.sin_addr.s_addr = inet_addr(argv[1]);//多播地址
    mult_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,(void*)&time_live,sizeof time_live);
    if((fp=fopen("news.txt","r"))==NULL){
        error_handling("fopen() error");
    }
    while(!feof(fp)){
        //标准文档给出的解释是：如果读取操作已尝试读取超过文件末尾的值， feof 函数将返回非零值;否则返回0
        fgets(buf,BUF_SIZE,fp);
        int test=sendto(send_sock,buf,strlen(buf),0,(struct sockaddr*)&mult_adr,sizeof mult_adr);
        printf("test:%d",test);
        //sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}
void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}