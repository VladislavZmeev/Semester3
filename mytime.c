#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>     // strsignal

int main(int argc, char *argv[])
{
    struct timeval start, end;
    struct rusage usage;
    int status;                         // статус завершения ребёнка

    gettimeofday(&start, NULL);

    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(argv[1], &argv[1]);

        // Сюда попадаем только если execvp завершился ошибкой
        perror("execvp");
        return 1;
    }

    // Раньше здесь был NULL вместо &status
    wait4(pid, &status, 0, &usage);

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

    // Анализируем, КАК завершился ребёнок
    if (WIFEXITED(status))
    {
        printf("Child exited normally, code = %d\n",
               WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);

        printf("Child was terminated by signal %d (%s)\n",
               sig, strsignal(sig));
    }

    return 0;
}