#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "common.h"

#define INTERVAL      3
#define FORMAT        "%a %H:%M"

int put_infos(char *format)
{
    char buf[MAXLEN];
    time_t ct;
    struct tm *lt;
    if ((ct = time(NULL)) != -1 &&
            (lt = localtime(&ct)) != NULL &&
            strftime(buf, sizeof(buf), format, lt) > 0 &&
            printf("%s\n", buf) > 0 &&
            fflush(stdout) != EOF)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    char *format = FORMAT;
    int interval = INTERVAL;
    bool snoop = false;

    char opt;
    while ((opt = getopt(argc, argv, "hsf:i:")) != -1) {
        switch (opt) {
            case 'h':
                printf("clock [-h|-s|-f FORMAT|-i INTERVAL]\n");
                exit(EXIT_SUCCESS);
                break;
            case 's':
                snoop = true;
                break;
            case 'f':
                format = optarg;
                break;
            case 'i':
                interval = atoi(optarg);
                break;
        }
    }

    int exit_code;

    if (snoop)
        while ((exit_code = put_infos(format)) != EXIT_FAILURE)
            sleep(interval);
    else
        exit_code = put_infos(format);

    return exit_code;
}
