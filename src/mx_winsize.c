#include "ush.h"

void mx_winsize(t_info *info) {
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    info->winsize = w.ws_col;
}
