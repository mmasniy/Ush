#include "../inc/ush.h"

static bool check_file_or_folder(t_info *info, char *head) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    int return_value = 0;

    if ((f = opendir(head))) {
        mx_print_error(info, ": is a directory", head);
        return_value = 126;
    }
    else {
        char *up_to_one = mx_up_to_one(head);
        char *file = mx_strdup(head + strlen(up_to_one));

        if ((f = opendir(up_to_one))) {
            while ((d = readdir(f)))
                if (strcmp(file, d->d_name) == 0)
                    break;
            mx_print_error(info, ": No such file or directory", head);
            return_value = (info->status = 127);
        }
    }
    f ? closedir(f) : 0;
    return (info->status = return_value);
}

void mx_check_error(t_info *i, char *error, char *arg) {
    if (mx_get_char_index(arg, '/') >= 0) {
        check_file_or_folder(i, arg);
    }
    else {
        mx_print_error(i, error, arg);
        i->status = 1;
    }
    exit(1);
}

void mx_print_error(t_info *i, char *error, char *arg) {
    if (error) {
        mx_printerr("u$h: ");
        mx_printerr(arg);
        mx_printerr(error);
    }
    mx_printerr("\n");
    exit(1);
    i->status = 1;
}

void mx_print_red_err(int i, t_info *info) {
    if (i == 0) {
        mx_printerr(MX_DOT);
    }
    else {
        mx_printerr(MX_RED);
        mx_printerr(MX_DOP_RED);
    }
    info->status = 1;
}
