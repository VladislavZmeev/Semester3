#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_NUM_OF_FILES 1024

int main(int argc, char* argv[])
{
    int is_l_flag = 0;
    char *dirname = ".";

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
    printf("\ndirname = %s, l = %d\n\n", dirname, is_l_flag);

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
        printf("%s\n", filename);
    }
    closedir(dir);
    return 0;
}