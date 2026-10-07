#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sleepsort(int argc, char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
        int x = atoi(argv[i]);
        int pid = fork();
        
        if (pid == 0)
        {
            sleep(x); //?
            printf( "%d", x);
            break;
        }


    }
    for(int i = 1; i < argc; i++)
    {
        wait(NULL);
    }
    return;

}

int main(int argc, char *argv[])
{
    sleepsort(argc, argv);

    return 0;
}