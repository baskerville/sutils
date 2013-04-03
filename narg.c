#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
        return EXIT_FAILURE;

    int n = atoi(argv[1]) - 1;
    if (n < 0)
        return EXIT_FAILURE;

    int c = argc - 2;

    printf("%s\n", argv[2 + (n % c)]);

    return EXIT_SUCCESS;
}
