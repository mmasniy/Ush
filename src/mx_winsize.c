#include "../inc/ush.h"

int mx_winsize(t_info *info) {
    if (tgetent(NULL, "xterm-256color") < 0) {
        fprintf(stderr, "ush: Could not access the termcap data base.\n");
        exit(1);
    }
    info->winsize = tgetnum("co");
    return info->winsize;
}
