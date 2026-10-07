#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
    struct timeval start, end;
    struct rusage usage;

    gettimeofday(&start, NULL);

    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return 1;
    }

    wait4(pid, NULL, 0, &usage);

    gettimeofday(&end, NULL);

    double real_time =
        end.tv_sec - start.tv_sec +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    double user_time =
        usage.ru_utime.tv_sec +
        usage.ru_utime.tv_usec / 1000000.0;

    double system_time =
        usage.ru_stime.tv_sec +
        usage.ru_stime.tv_usec / 1000000.0;

    printf("real:   %.3f sec\n", real_time);
    printf("user:   %.3f sec\n", user_time);
    printf("system: %.3f sec\n", system_time);

    return 0;
}