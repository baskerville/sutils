#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sensors/sensors.h>
#include <sensors/error.h>
#include "common.h"

#define INTERVAL 3
#define FORMAT "%s %4.0f "

int put_infos(char *format)
{
    int chip_nr = 0;

    const sensors_chip_name *chip;

    while ((chip = sensors_get_detected_chips(NULL, &chip_nr))) {

        int i = 0;
        const sensors_feature *feature;

        while ((feature = sensors_get_features(chip, &i))) {

            const sensors_subfeature *sf;

            if (feature->type == SENSORS_FEATURE_FAN) {
                double val;
                char *label = NULL;

                label = sensors_get_label(chip, feature);

                sf = sensors_get_subfeature(chip, feature, 
                        SENSORS_SUBFEATURE_FAN_FAULT);
                if (sf && sensors_get_value(chip, sf->number, &val)) {
                    val = -1;
                }
                else {
                    sf = sensors_get_subfeature(chip, feature,
                            SENSORS_SUBFEATURE_FAN_INPUT);
                    if (sensors_get_value(chip, sf->number, &val)) {
                        val = -1;
                    }
                }

                if (strstr(format, "%s") == NULL) {
                    printf(format, val);
                }
                else if (strpbrk(format, "%f") == NULL) {
                    printf(format, label);
                }
                else {
                    printf(format, label, val);
                }
                free(label);
            }
        }
    }

    printf("\n");
    fflush(stdout);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    char *format = FORMAT;
    int interval = INTERVAL;
    bool snoop   = false;
    int exit_code;

    char opt;

    while ((opt = getopt(argc, argv, "hsf:i:")) != -1) {
        switch (opt) {
            case 'h':
                printf("fan_speed [-h|-s|-f FORMAT|-i INTERVAL]\n");
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

    exit_code = sensors_init(NULL);
    if (exit_code) {
        fprintf(stderr, "sensors_init: %s\n", sensors_strerror(exit_code));
        return exit_code;
    }

    if (snoop) {
        while ((exit_code = put_infos(format)) != EXIT_FAILURE) {
            sleep(interval);
        }
    }
    else {
        exit_code = put_infos(format);
    }

    sensors_cleanup();
    return exit_code;
}
