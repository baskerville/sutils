#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

#include <alsa/asoundlib.h>
#include <alsa/control.h>

#define INTERVAL       3
#define FORMAT         "%s %i"
#define DEVICE         "hw:0"
#define VOLUME_CONTROL "Master Playback Volume"
#define MUTE_CONTROL   "Master Playback Switch"

int put_infos(int vol_min, int vol_max,
    snd_hctl_elem_t *volume_elem, snd_hctl_elem_t *mute_elem,
    snd_ctl_elem_value_t *volume_ctl, snd_ctl_elem_value_t *mute_ctl,
    char *format)
{
    int volume;
    int volume_percent;
    bool mute_switch;
    char switch_state[4];

    snd_hctl_elem_read(volume_elem, volume_ctl);
    snd_hctl_elem_read(mute_elem, mute_ctl);

    volume = (int)snd_ctl_elem_value_get_integer(volume_ctl, 0);
    mute_switch = (bool)snd_ctl_elem_value_get_boolean(mute_ctl, 0);

    volume_percent = (int)(100.0f * ((float)volume - vol_min) / (vol_max - vol_min));

    if (mute_switch == true)
        sprintf(switch_state, "on");
    else
        sprintf(switch_state, "off");

    if (strstr(format, "%s") == NULL)
        printf(format, volume_percent);
    else if (strstr(format, "%i") == NULL)
        printf(format, switch_state);
    else
        printf(format, switch_state, volume_percent);
    printf("\n");
    fflush(stdout);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    char *format = FORMAT;
    char *device = DEVICE;
    char *volume_control = VOLUME_CONTROL;
    char *mute_control = MUTE_CONTROL;
    int interval = INTERVAL;
    bool snoop = false;

    char opt;
    while ((opt = getopt(argc, argv, "hsf:i:d:v:m:")) != -1) {
        switch (opt) {
            case 'h':
                printf("battery [-h|-s|-f FORMAT|-i INTERVAL|-d DEVICE|-v VOLUME_CONTROL|-m MUTE_CONTROL]\n");
                exit(EXIT_SUCCESS);
                break;
            case 's':
                snoop = true;
                break;
            case 'f':
                format = optarg;
                break;
            case 'd':
                device = optarg;
                break;
            case 'i':
                interval = atoi(optarg);
                break;
            case 'v':
                volume_control = optarg;
                break;
            case 'm':
                mute_control = optarg;
                break;
        }
    }

    int exit_code;

    snd_hctl_t *hctl;
    snd_ctl_elem_id_t *volume_id;
    snd_ctl_elem_id_t *mute_id;
    snd_ctl_elem_value_t *volume_ctl;
    snd_ctl_elem_value_t *mute_ctl;
    snd_ctl_elem_info_t *volume_info;
    snd_hctl_elem_t *volume_elem;
    snd_hctl_elem_t *mute_elem;
    int vol_max, vol_min;

    if (snd_hctl_open(&hctl, device, 0) || snd_hctl_load(hctl)) {
        return EXIT_FAILURE;
    }

    snd_ctl_elem_id_malloc(&volume_id);
    snd_ctl_elem_id_malloc(&mute_id);
    snd_ctl_elem_value_malloc(&volume_ctl);
    snd_ctl_elem_value_malloc(&mute_ctl);
    snd_ctl_elem_info_malloc(&volume_info);

    snd_ctl_elem_id_set_interface(volume_id, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_id_set_interface(mute_id, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_id_set_name(volume_id, volume_control);
    snd_ctl_elem_id_set_name(mute_id, mute_control);

    snd_ctl_elem_value_set_id(volume_ctl, volume_id);
    snd_ctl_elem_value_set_id(mute_ctl, mute_id);
    volume_elem = snd_hctl_find_elem(hctl, volume_id);
    mute_elem = snd_hctl_find_elem(hctl, mute_id);

    snd_ctl_elem_info_set_id(volume_info, volume_id);
    snd_hctl_elem_info(volume_elem, volume_info);
    vol_min = (int)snd_ctl_elem_info_get_min(volume_info);
    vol_max = (int)snd_ctl_elem_info_get_max(volume_info);


    if (volume_elem == NULL || mute_elem == NULL) {
        snd_hctl_close(hctl);
        return EXIT_FAILURE;
    }

    if (snoop)
        while ((exit_code = put_infos(vol_min, vol_max, volume_elem, mute_elem,
            volume_ctl, mute_ctl, format)) != EXIT_FAILURE)
            sleep(interval);
    else
        exit_code = put_infos(vol_min, vol_max, volume_elem, mute_elem,
            volume_ctl, mute_ctl, format);

    snd_hctl_close(hctl);
    snd_ctl_elem_id_free(volume_id);
    snd_ctl_elem_id_free(mute_id);
    snd_ctl_elem_value_free(volume_ctl);
    snd_ctl_elem_value_free(mute_ctl);
    snd_ctl_elem_info_free(volume_info);
    return exit_code;
}

