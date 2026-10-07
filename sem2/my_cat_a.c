#include <unistd.h>   /* read, write, close */
#include <fcntl.h>    /* open, O_RDONLY */
#include <stdio.h>    /* perror */
#include <stdlib.h>   /* exit, EXIT_FAILURE */
#define BUF_SIZE 1024
int main(int argc, char* argv[])
{
	char buf[BUF_SIZE];
	if (argc == 1)
	{
		ssize_t n = 0;
		while ((n = read(0, buf, sizeof(buf))) > 0)
			write(1, buf, n);
		return 0;		
	}
	for (int i = 1; i < argc; i++)
	{
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0)
		{
			perror(argv[i]);
			continue;
		}
		ssize_t n = 0;
		while ((n = read(fd, buf, sizeof(buf))) > 0)
			write(1, buf, n);
		close(fd);
	}

return 0;
}
