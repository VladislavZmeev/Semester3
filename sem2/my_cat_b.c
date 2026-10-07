#include <stdio.h>    /* fopen, fread, fwrite, fclose, perror */
#include <stdlib.h>   /* exit, EXIT_FAILURE */

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];

    if (argc < 2) {
        size_t n;
        while ((n = fread(buf, 1, sizeof(buf), stdin)) > 0)
            fwrite(buf, 1, n, stdout);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (!fp) {
            perror(argv[i]);
            continue;
        }

        size_t n;
        while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
            fwrite(buf, 1, n, stdout);

        fclose(fp);
    }

    return 0;
}
