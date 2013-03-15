/* Filter duplicate lines from standard input */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int main(void) {
    char cur[BUFSIZ] = {0};
    char last[BUFSIZ] = {0};
    int nr = 1, nw = 1;
    while (nr > 0 && nw == nr) {
        if ((nr = read(STDIN_FILENO, cur, sizeof(cur))) > 0) {
            cur[nr] = 0;
            if (strcmp(cur, last) != 0) {
                nw = write(STDOUT_FILENO, cur, nr);
                strcpy(last, cur);
            }
        }
    }
    if (nr > 0 && nw <= 0)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
