#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <errno.h>
#include <linux/wireless.h>

#define INTERVAL   5
#define INTERFACE  "wlan0"
#define FORMAT     "%s"

char *format = FORMAT;
int interval = INTERVAL;
char name[IW_ESSID_MAX_SIZE + 1] = {0};

void put_status(int fd, struct iwreq *rqt)
{
    if (ioctl(fd, SIOCGIWESSID, rqt) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }
    printf(format, name);
    printf("\n");
}

int main(int argc, char *argv[])
{
    char *interface = INTERFACE;
    bool snoop = false;

    char opt;
    while ((opt = getopt(argc, argv, "hsf:i:w:")) != -1) {
        switch (opt) {
            case 'h':
                printf("essid [-h|-s|-i INTERVAL|-f FORMAT|-w INTERFACE]\n");
                exit(EXIT_SUCCESS);
                break;
            case 'i':
                interval = atoi(optarg);
                break;
            case 's':
                snoop = true;
                break;
            case 'f':
                format = optarg;
                break;
            case 'w':
                interface = optarg;
                break;
        }
    }

    struct iwreq request;
    int sock_fd;
    memset(&request, 0, sizeof(struct iwreq));
    sprintf(request.ifr_name, interface);

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    request.u.essid.pointer = name;
    request.u.essid.length = IW_ESSID_MAX_SIZE + 1;

    if (snoop)
        while (true) {
            put_status(sock_fd, &request);
            sleep(interval);
            name[0] = '\0';
        }
    else
        put_status(sock_fd, &request);

    close(sock_fd);
    if (strlen(name) > 0)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
