#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    char **args = argv + 1;
    int num = argc - 1;

    if (num != 2) {
        fprintf(stderr, "usage: %s STRFTIME_FORMAT SLEEP_INTERVAL\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *format = *args;
    int interval = atoi(*(args + 1));

    char buf[BUFSIZ];
    time_t ct;
    struct tm *lt;

    while (true) {
        ct = time(NULL);
        lt = localtime(&ct);
        strftime(buf, sizeof(buf), format, lt);
        if (printf("%s\n", buf) < 0)
            return EXIT_FAILURE;
        fflush(stdout);
        sleep(interval);
    }

    return EXIT_SUCCESS;
}
