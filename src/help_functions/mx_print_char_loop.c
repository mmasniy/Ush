#include "../../inc/ush.h"

void mx_print_char_loop(char c, int len) {
    for (int i = len; i > 0; i--)
        write(STDOUT_FILENO, &c, 1);
}
