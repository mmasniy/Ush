#include "../../inc/ush.h"

static void functions_search(t_info *info, char *what_check) {
    char *binary_func = NULL;
    char **all_binary_func;

    if ((binary_func = mx_find_similar_buildin(what_check))) {
        mx_push_history_back(&info->tab_list, binary_func);
        mx_strdel(&binary_func);
    }
    if ((binary_func = mx_find_in_PATH(info->paths, what_check, 0))) {
        all_binary_func = mx_strsplit(binary_func, ':');
        for (int i = 0; all_binary_func[i]; i++)
            mx_push_history_back(&info->tab_list, all_binary_func[i]);
        mx_del_strarr(&all_binary_func);
        mx_strdel(&binary_func);
    }
    mx_replace_special_symbols(info);
    info->tab_pos = info->tab_list->next;
}

static void check_if_that_folder(t_info *info, char *what_check
    , DIR *f, struct dirent *d) {
    char *full_path = NULL;
    char *second = NULL;

    if (what_check[mx_strlen(what_check) - 1] != '/')
        full_path = mx_strjoin(what_check, "/");
    else
        full_path = what_check;
    while ((d = readdir(f))) {
        if (strcmp(d->d_name, ".") && strcmp(d->d_name, "..")) {
            second = mx_strjoin(full_path, d->d_name);
            mx_push_history_back(&info->tab_list, second);
            mx_strdel(&second);
        }
    }
    closedir(f);
}

static void check_for_file(t_info *info, char *wrd, DIR *f, struct dirent *d) {
    char *file = mx_up_to_one(wrd);
    char *snt = NULL;
    int ln = strlen(file);

    for (int i = 0; (i = mx_get_char_index(wrd + ln, '/')) >= 0; ln += i + 1);
    if (mx_get_char_index(wrd, '/') != -1 && (f = opendir(file))) {
        while ((d = readdir(f)))
            if (!mx_str_head(d->d_name, &wrd[ln])) {
                snt = strcmp("/", file) ? mx_strjoin(file, "/") : strdup(file);
                mx_del_and_set(&snt, mx_strjoin(snt, d->d_name));
                mx_push_history_back(&info->tab_list, snt);
                mx_strdel(&snt);
            }
    }
    else if ((f = opendir(".")))
        while ((d = readdir(f)))
            if (strcmp(d->d_name, ".") && strcmp(d->d_name, "..")
                && mx_str_head(d->d_name, wrd) == 0)
                mx_push_history_back(&info->tab_list, d->d_name);
    f ? closedir(f) : 0;
}

static void create_new_tab_list(t_info *i, char *check, char **buf, int *pos) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    char *home_path = mx_strjoin(getenv("HOME"), check + 1);

    while (i->tab_list)
        mx_pop_history_front(&i->tab_list);
    mx_push_history_back(&i->tab_list, check);
    if (check[0] == '~')
        if ((f = opendir(home_path)))
            check_if_that_folder(i, check, f, d);
    mx_strdel(&home_path);
    (f = opendir(check)) ? check_if_that_folder(i, check, f, d) : 0;
    if (mx_str_char_in_str(*buf, " \t\r\n\f\v"))
        check_for_file(i, check, f, d);
    functions_search(i, check);
    i->tab_pos = i->tab_list;
    if (i->tab_list && i->tab_list->next && i->tab_list->next->next)
        mx_print_tab_list(i);
    else
        i->tab_list && i->tab_list->next ? mx_tab_work(i, buf, pos) : 0;
}

void mx_tab_work(t_info *i, char **b, int *position) {
    int p = *position - 1;
    int len = 1;
    char *w = NULL;

    for (; p > 0 && (!mx_isspace((*b)[p - 1])
         || (p > 1 && (*b)[p - 2] == '\\')); p--, len++);
    w = strndup(&(*b)[p], len);
    if (i->tab_list && i->tab_list->next && !strcmp(w, i->tab_pos->data)) {
        i->tab_pos = i->tab_pos->next;
        if (!i->tab_pos)
            i->tab_pos = i->tab_list->next;
        for (char c = 127; *position > p; )
            mx_str_edit(i, *b, position, &c);
        for (int j = 0; j < mx_strlen(i->tab_pos->data); j++)
            mx_str_edit(i, *b, &p, &(i->tab_pos->data[j]));
        *position += mx_strlen(i->tab_pos->data);
    }
    else // Create new
        create_new_tab_list(i, w, b, position);
    mx_strdel(&w);
}
