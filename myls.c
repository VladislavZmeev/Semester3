#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_NUM_OF_FILES 1024

int main(int argc, char* argv[])
{
    int is_l_flag = 0;
    char *dirname = ".";
    char path[1024];

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
        {
            is_l_flag = 1;
        }
        else
        {
            dirname = argv[i];
        }
    }
    //printf("\ndirname = %s, l = %d\n\n", dirname, is_l_flag);

    struct dirent *entry;

    DIR *dir;
    dir = opendir(dirname);
    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while((entry = readdir(dir)) != NULL)
    {
        char* filename = entry->d_name;
        if (filename[0] == '.')
        {
            continue;
        }
        if (is_l_flag)
        {
            snprintf(path, sizeof(path), "%s/%s", dirname, filename);
            //snprintf — это функция для записи форматированной строки в буфер с ограничением по размеру.
            
            struct stat st;
            if (stat(path, &st) == -1)
            {
                perror("stat");
                continue;
            }

            printf("%-5ld %s\n", st.st_size, filename);
        }
        else
        {
            printf("%s\n", filename);
        }
    }
    closedir(dir);
    return 0;
}