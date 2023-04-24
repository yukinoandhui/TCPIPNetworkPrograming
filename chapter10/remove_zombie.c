#define _XOPEN_SOURCE
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#
#include <stdlib.h>
#include <unistd.h>
#
void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Remove process id:%d\n", id);
        printf("child send : %d\n", WEXITSTATUS(status));
    }
}
int main()
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    pid = fork();
    if (pid == 0) {
        puts("Hi! I am child process");
        sleep(10);
        return 12;
    } else {
        printf("child process id:%d\n", pid);
        pid = fork();
        if (pid == 0) {
            puts("Hi! I am child process");
            sleep(10);
            exit(24);
        } else {
            int i;
            printf("child proc id: %d \n", pid);
            for (i = 0; i < 5; i++) {
                puts("wait...");
                sleep(5);
            }
        }
    }
    return 0;
}