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
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    len = sizeof snd_buf;

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);

    if (state) {
        error_handling("getsockopt() error");
    }
    len = sizeof rcv_buf;

    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);

    if (state) {
        error_handling("getsockopt() error");
    }
    printf("Input buffer size: %d\n",rcv_buf);
    printf("Output buffer size: %d\n",snd_buf);
    return 0;
}
void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}