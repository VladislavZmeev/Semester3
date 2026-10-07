#include <stdio.h>
#include <unistd.h>

void print_process_info(int fork_result)
{
    printf("PID = %d PPID = %d, ", getpid(), getppid());
    if (fork_result == 0)
    {
        printf("I'm child\n");
    }
    else
    {
        printf("I'm parent\n");
    }
}
int main()
{
    int pid = fork();
    print_process_info(pid);

    int n = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        pid = fork();
        
        if (pid == 0)
        {
            print_process_info(pid);
            break;
        }
    }

    return 0;
}