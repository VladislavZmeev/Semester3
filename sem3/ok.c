#include <stdio.h>
#include <unistd.h>
int main()
{
    printf("Hello ");
    sleep(2);
    fork();
    printf("world! ");
    return 0;
}