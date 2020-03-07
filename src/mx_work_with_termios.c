#include "../inc/ush.h"

bool mx_custom_termios(t_info *info, int fd) {
    struct termios raw;

    if (tcgetattr(fd, &info->origin_termios) == -1) {
        return 0;
    }
    raw = info->origin_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST); // with that baground process can print wrong
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSAFLUSH, &raw) < 0)
        return 0;
    return 1;
}

bool mx_origin_termios(t_info *info, int fd) {
    tcsetattr(fd, TCSAFLUSH, &info->origin_termios);
    return 1;
}
