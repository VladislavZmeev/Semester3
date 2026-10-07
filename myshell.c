#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define TOK_SIZE 64
//int argc, char* argv[]
int main() 
{
    char buf[BUF_SIZE];
    //char buf_copy[BUF_SIZE];

    printf("myshell$ ");
    fflush(stdout);
 
    while(fgets(buf, sizeof(buf), stdin) != NULL) 
    {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
        {
            buf[len - 1] = '\0';
        }
        if (strcmp(buf, "myexit")   == 0)
        {
            break;
        }
        
        char* token[TOK_SIZE];
        token[TOK_SIZE - 1] = NULL; // гарантируем то, что в конце будет NULL
        token[0] = strtok(buf, " \t");  // Делит строку на части по разделителям, изменяя исходную строку

        if (token[0] == NULL)
        {
            printf("myshell$ ");
            fflush(stdout);
            continue;
        }
        //int ntokens = 1;
        //printf("token[0] -> %s\n", token[0]);
        for (int i = 1; i < TOK_SIZE - 1; i++)
        {
            token[i]= strtok(NULL, " \t"); // Получает следующий фрагмент той же строки
            if (token[i] == NULL)
            {
                break;
            }
            //printf("token[%d] -> %s\n", i, token[i]);
            //ntokens += 1;
        }
        
        pid_t pid = fork();
        if (pid == 0)
        {
            execvp(token[0], token);
            //если execvp успешен, код дальше не выполняеся
            perror("execvp");
            _exit(1); //чтобы дочерний процесс завершился и не превратился во второй myshell
        }
        if (pid < 0)
        {
            perror("fork");
        }
        if (pid > 0)
        {
            waitpid(pid, NULL, 0);
        }
        printf("myshell$ ");
        fflush(stdout); //Немедленно выводит данные из буфера stdout

        
    }
    putchar('\n');
}


// before strtok:
// "ls -l /tmp"

// after strtok:
// "ls\0-l\0/tmp\0"