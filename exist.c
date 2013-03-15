#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/stat.h>

void check_file(char *pth, bool inv) {
    struct stat buf;
    int ret = stat(pth, &buf);
    if ((ret == 0 && !inv) || (ret == -1 && inv))
        puts(pth);
}

int main(int argc, char *argv[]) {
    char opt;
    bool invert_matches = false;

    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
            case 'h':
                printf("exist [-hv] [FILES]\n");
                return EXIT_SUCCESS;
                break;
            case 'v':
                invert_matches = true;
                break;
        }
    }

    int num = argc - optind;
    char **args = argv + optind;

    if (num > 0) {
        int i;
        for (i = 0; i < num; i++)
            check_file(args[i], invert_matches);
    } else {
        char line[BUFSIZ] = {0};
        while (fgets(line, sizeof(line), stdin) != NULL) {
            int last = strlen(line) - 1;
            if (last >= 0 && line[last] == '\n')
                line[last] = 0;
            check_file(line, invert_matches);
        }
    }

    return EXIT_SUCCESS;
}
