#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    int fds[2];
    pipe(fds);
    if (pipe(fds) < 0) {
        perror("pipe");
    }

    pid_t pid = fork();

    if (pid == 0) // child
    {
        char massage[] = "Hello, World!";
        write(fds[1], massage, sizeof(massage));
        close(fds[1]);
        close(fds[0]);
    }
    else // parent
    {
        char buf[100]; 
        read(fds[0], buf, sizeof(buf));
        printf("%s\n", buf);
        close(fds[0]);
        close(fds[1]);
    }
    return;

}