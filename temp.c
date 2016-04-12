#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

#define INTERVAL 3
#define FORMAT "%3.1f"
#define TEMP_PATH "/sys/bus/acpi/devices/LNXTHERM:%02d/thermal_zone/temp"
#define TEMP_INDEX 00

int put_info(char *path, char *format)
{
    FILE *fp = fopen(path, "r");

    if (!fp) {
        fprintf(stderr, "Can't open '%s'.\n", path);
        return EXIT_FAILURE;
    }

    float temp;

    fscanf(fp, "%f", &temp);
    fclose(fp);

    printf(format, temp / 1000);
    printf("\n");
    fflush(stdout);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    char *path = TEMP_PATH;
    char *format = FORMAT;
    int index = TEMP_INDEX;
    int interval = INTERVAL;
    bool snoop = false;

    int opt;
    while ((opt = getopt(argc, argv, "hsf:i:p:n:")) != -1) {
        switch (opt) {
        case 'h':
            printf("temp [-h|-s|-f FORMAT|-i INTERVAL|-p PATH|-n INDEX]\n");
            return EXIT_SUCCESS;
        case 's':
            snoop = true;
            break;
        case 'f':
            format = optarg;
            break;
        case 'i':
            interval = atoi(optarg);
            break;
        case 'p':
            path = optarg;
            break;
        case 'n':
            index = atoi(optarg);
            break;
        }
    }

    char real_path[MAXLEN] = {0};
    snprintf(real_path, sizeof(real_path), path, index);

    int exit_code;

    if (snoop)
        while ((exit_code = put_info(real_path, format)) != EXIT_FAILURE)
            sleep(interval);
    else
        exit_code = put_info(real_path, format);

    return exit_code;
}
