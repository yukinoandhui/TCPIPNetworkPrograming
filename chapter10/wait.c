#include <stdio.h>
#include <sys/types.h> //防止pid_t未定义
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        return 3;
    } else {
        printf("child process Id: %d \n", pid);
        wait(&status); // 等价于waitpid(-1,&status,WNOHANG)
        if (WIFEXITED(status)) {
            printf("child send one: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}