#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
int main(){
    //O_CREAT
    struct in_addr x;
    //转换为网络字节
    x.s_addr= inet_addr("192.168.0.1");
    //需要复制一份转换的结果，因为tem_ip的内存实际是函数内部分配的
    char * tem_ip= inet_ntoa(x);
    char strarr[20];
    strcpy(strarr,tem_ip);

    return 0;
}