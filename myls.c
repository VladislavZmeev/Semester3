#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

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

    

    DIR *dir;
    dir = opendir(dirname);
    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
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
            
            //вывод тип d/-/?
            struct stat st;
            if (stat(path, &st) == -1)
            {
                perror("stat");
                continue;
            }

            if (S_ISDIR(st.st_mode)) 
            {
            putchar('d');
            }    
            else if (S_ISREG(st.st_mode))
            {
                putchar('-');
            }
            else
            {
                putchar('?');
            }

            //вывод права rwx
            putchar(st.st_mode & S_IRUSR ? 'r' : '-');
            putchar(st.st_mode & S_IWUSR ? 'w' : '-');
            putchar(st.st_mode & S_IXUSR ? 'x' : '-');
            putchar(st.st_mode & S_IRGRP ? 'r' : '-');
            putchar(st.st_mode & S_IWGRP ? 'w' : '-');
            putchar(st.st_mode & S_IXGRP ? 'x' : '-');
            putchar(st.st_mode & S_IROTH ? 'r' : '-');
            putchar(st.st_mode & S_IWOTH ? 'w' : '-');
            putchar(st.st_mode & S_IXOTH ? 'x' : '-');
            putchar(' ');

            //вывод числа ссылок
            printf("%d ", (int)st.st_nlink);

            //вывод user & group
            struct passwd *pw;
            struct group *gr;
            pw = getpwuid(st.st_uid);
            gr = getgrgid(st.st_gid);
            printf("%s %s ", pw->pw_name, gr->gr_name);

            //вывод размера
            printf("%6ld ", st.st_size);

            //вывод mounth/day/time
            struct tm *tm_info;
            tm_info = localtime(&st.st_mtime);

            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
            printf("%s ", timebuf);
        }
        printf("%s\n", filename);
    }
    closedir(dir);
    return 0;
}