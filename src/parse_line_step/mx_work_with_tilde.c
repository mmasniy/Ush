#include "../../inc/ush.h"

static bool before_tilde(char *line, int pos) {
    if (pos == 0 || (mx_strchr(MX_TYPE, line[pos - 1])
        && (pos == 1 || line[pos - 2] != '\\')))
        return 1;
    return 0;
}

static bool read_user(char *user, char **tilde_change) {
    struct passwd *find = getpwnam(user);

    if (find) {
        mx_strdel(tilde_change);
        *tilde_change = mx_strdup(find->pw_dir);
        return 0;
    }
    return 1;
}

static bool read_tilde_content(char *line, int *pos, char **tilde_change) {
    char *check_line = NULL;
    int size = 1;

    for (int i = *pos + 1; line[i + size]; size++)
        if (line[i + size] == '/' || mx_strchr(MX_TYPE, line[i + size]))
            break;
    check_line = strndup(&(line[*pos + 1]), size);
    if (!(*tilde_change))
        if (read_user(check_line, tilde_change))
            mx_del_and_set(tilde_change, mx_strjoin("~", check_line));
    mx_strdel(&check_line);
    *pos += size;
    return 0;
}

static void check_what_want(t_info *info, char *craft, char **new, int *pos) {
    char *tilde_change = NULL;

    if (!craft[*pos + 1] || mx_strchr(MX_TYPE, craft[*pos + 1])
        || craft[*pos + 1] == '/')
        tilde_change = strdup(getenv("HOME"));
    else if (craft[*pos + 1] == '-'&& ((!craft[*pos + 2])
        || mx_isspace(craft[*pos + 2])) && (++(*pos)))
        tilde_change = strdup(info->oldpwd);
    else if (craft[*pos + 1] == '+' && ((!craft[*pos + 2])
        || mx_isspace(craft[*pos + 2])) && (++(*pos)))
        tilde_change = strdup(info->pwd);
    else
        read_tilde_content(craft, pos, &tilde_change);
    if (tilde_change)
        mx_del_and_set(new, mx_strjoin(*new, tilde_change));
    mx_strdel(&tilde_change);
}

/*
* end = '/', ' ', '\0'
* can be : a-z, A-Z, -, _, 0-9 and just like all sub (+).
* ~   = /Users/vkmetyk
* ~/  = /Users/vkmetyk/
* ~-  = old_pwd
* ~+  = this_folder
* ~file_in_Users_folder
* if (~file_in_Users_folder == not exist)
* print = u$h: no such user or named directory: file_in_Users_folder
*/

int mx_tilde_work(t_info *info, char **line, char *craft) {
    int pos = 0;
    char *new_line = mx_strnew(strlen(*line));
    char *tmp = NULL;

    for (int i = 0; (i = mx_get_char_index(&(craft[pos]), '~')) >= 0; pos++) {
        tmp = strndup(&(craft[pos]), i);
        mx_del_and_set(&new_line, mx_strjoin(new_line, tmp));
        mx_strdel(&tmp);
        pos += i;
        if (mx_is_quotes(craft, pos) != '\'' && before_tilde(craft, pos))
            check_what_want(info, craft, &new_line, &pos);
        else
            mx_del_and_set(&new_line, mx_strjoin(new_line, "~"));
    }
    mx_del_and_set(&new_line, mx_strjoin(new_line, mx_strdup(&(craft[pos]))));
    mx_del_and_set(line, strdup(new_line));
    mx_strdel(&new_line);
    return 0;
}
