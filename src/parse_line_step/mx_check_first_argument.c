#include "../../inc/ush.h"

static bool check_file_or_folder(t_info *info, char *head) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    int return_value = 0;

    if ((f = opendir(head))) {
        mx_print_error(info, head, ": is a directory\n");
        return_value = 126;
    }
    else {
        char *up_to_one = mx_up_to_one(head);
        char *file = mx_strdup(head + strlen(up_to_one));

        if ((f = opendir(up_to_one))) {
            while ((d = readdir(f)))
                if (strcmp(file, d->d_name) == 0)
                    break;
            mx_print_error(info, head, ": No such file or directory\n");
            return_value = (info->status = 127);
        }
    }
    f ? closedir(f) : 0;
    return (info->status = return_value);
}

bool mx_check_first_argument(t_info *info, char *head) {
    if (mx_get_char_index(head, '/') >= 0) {
        return check_file_or_folder(info, head);
    }
    else {
        char *tmp = mx_find_in_PATH(info->paths, head, 1);

        if (tmp || mx_check_buildin(info, 0) == 0)
            mx_strdel(&tmp);
        else {
            mx_print_error(info, head, ": command not found");
            return (info->status = 1);
        }
    }
    return 0;
}
