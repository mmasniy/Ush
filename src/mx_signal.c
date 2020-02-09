#include "../inc/ush.h"

void mx_sigio_handler(int sigio) {
    mx_printchar('\n');
    (void)sigio;
}
