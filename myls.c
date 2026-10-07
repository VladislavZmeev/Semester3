#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_permissions(mode_t mode)
{
    // Тип файла
    if (S_ISDIR(mode))
        putchar('d');
    else if (S_ISLNK(mode))
        putchar('l');
    else if (S_ISREG(mode))
        putchar('-');
    else
        putchar('?');

    // user
    putchar(mode & S_IRUSR ? 'r' : '-');
    putchar(mode & S_IWUSR ? 'w' : '-');
    putchar(mode & S_IXUSR ? 'x' : '-');

    // group
    putchar(mode & S_IRGRP ? 'r' : '-');
    putchar(mode & S_IWGRP ? 'w' : '-');
    putchar(mode & S_IXGRP ? 'x' : '-');

    // other
    putchar(mode & S_IROTH ? 'r' : '-');
    putchar(mode & S_IWOTH ? 'w' : '-');
    putchar(mode & S_IXOTH ? 'x' : '-');
}


int main(int argc, char *argv[])
{
    char *dirname;

    if (argc > 1)
        dirname = argv[1];
    else
        dirname = ".";

    DIR *dir = opendir(dirname);

    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        // Как обычный ls без -a — скрытые файлы пропускаем
        if (entry->d_name[0] == '.')
            continue;

        char path[1024];

        snprintf(path, sizeof(path), "%s/%s",
                 dirname, entry->d_name);

        struct stat st;

        // lstat, чтобы символическая ссылка определялась именно как ссылка
        if (lstat(path, &st) < 0)
        {
            perror("lstat");
            continue;
        }

        // 1. Тип + права
        print_permissions(st.st_mode);

        // 2. Число hard links
        printf(" %2lu", (unsigned long)st.st_nlink);

        // 3. Пользователь
        struct passwd *pw = getpwuid(st.st_uid);

        if (pw != NULL)
            printf(" %-8s", pw->pw_name);
        else
            printf(" %-8u", st.st_uid);

        // 4. Группа
        struct group *gr = getgrgid(st.st_gid);

        if (gr != NULL)
            printf(" %-8s", gr->gr_name);
        else
            printf(" %-8u", st.st_gid);

        // 5. Размер
        printf(" %8ld", (long)st.st_size);

        // 6. Время последнего изменения
        char timebuf[64];

        struct tm *tm = localtime(&st.st_mtime);

        strftime(timebuf, sizeof(timebuf),
                 "%b %d %H:%M", tm);

        printf(" %s", timebuf);

        // 7. Имя
        printf(" %s\n", entry->d_name);
    }

    closedir(dir);

    return 0;
}