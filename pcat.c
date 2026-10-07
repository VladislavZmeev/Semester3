#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    
    int fds[2];
    pipe(fds);

    pid_t pid = fork();

    if (pid == 0)
    {
        //child: файл -> pipe
        int fd = open(argv[1], O_RDONLY);
        close(fds[0]);
        char buf[1024];
        int n = 0;
        while((n = read(fd, buf, sizeof(buf))) > 0)
        {
            write(fds[1], buf, n);
        }
        close(fd);
        close(fds[1]);
        _exit(0);
    }
    //parent: pipe -> stdout

    close(fds[1]);

    char buf[1024];
    int n = 0;

    while ((n = read(fds[0], buf, sizeof(buf))) > 0)
    {
        write(1, buf, n);
    }
    close(fds[0]);
    wait(NULL);

    return 0;

}