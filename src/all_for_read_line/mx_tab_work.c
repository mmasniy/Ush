#include "../../inc/ush.h"

static void check_for_file(t_info *info, char *wrd, DIR *f, struct dirent *d);
static void check_if_that_folder(t_info *info, char *what_check
    , DIR *f, struct dirent *d);
static void functions_search(t_info *info, char *what_check);
static void create_new_tab_list(t_info *info, char *what_check,
    char **buffer, int *position);

void mx_tab_work(t_info *info, char **buffer, int *position) {
    int pos = *position - 1;
    int len = 1;
    char *what_check = NULL;

    for (; pos > 0 && !mx_isspace((*buffer)[pos - 1]); pos--, len++);
    what_check = strndup(&(*buffer)[pos], len);
    if (info->tab_list && info->tab_list->next &&
    !strcmp(what_check, info->tab_pos->data)) { ////// If exist
        info->tab_pos = info->tab_pos->next;
        if (!info->tab_pos)
            info->tab_pos = info->tab_list->next;
        for (char c = 127; *position > pos; )
            mx_str_edit(info, *buffer, position, &c);
        for (int i = 0; i < mx_strlen(info->tab_pos->data); i++)
            mx_str_edit(info, *buffer, &pos, &(info->tab_pos->data[i]));
        *position += mx_strlen(info->tab_pos->data);
    }
    else { //////// Create new
        create_new_tab_list(info, what_check, buffer, position);
        mx_print_tab_list(info);
    }
}

static void check_for_file(t_info *info, char *wrd, DIR *f, struct dirent *d) {
    char *file = mx_up_to_one(wrd);
    char *sent = NULL;

    if ((f = opendir(file))) {
        while ((d = readdir(f)))
            if (!mx_str_head(d->d_name, &wrd[strlen(file)])) {
                sent = mx_strjoin(file, d->d_name);
                mx_push_history_back(&info->tab_list, sent);
                if (malloc_size(sent))
                    free(sent);
            }
    }
    else if ((f = opendir("."))) {
        mx_push_history_back(&info->tab_list, wrd);
        while ((d = readdir(f)))
            if (mx_str_head(d->d_name, wrd) == 0)
                mx_push_history_back(&info->tab_list, d->d_name);
        closedir(f);
    }
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
        second = mx_strjoin(full_path, d->d_name);
        mx_push_history_back(&info->tab_list, second);
    }
    closedir(f);
}

static void create_new_tab_list(t_info *info
    , char *what_check, char **buffer, int *position) {
    DIR *f = NULL;
    struct dirent *d = NULL;
    char *home_path = mx_strjoin(getenv("HOME"), what_check + 1);

    while (info->tab_list)
        mx_pop_history_front(&info->tab_list);
    mx_push_history_back(&info->tab_list, what_check);
    if (what_check[0] == '~')
        if ((f = opendir(home_path))) // check_if_that_folder
            check_if_that_folder(info, what_check, f, d);
    mx_strdel(&home_path);
    if ((f = opendir(what_check))) // check_if_that_folder
        check_if_that_folder(info, what_check, f, d);
    else if (mx_str_char_in_str(*buffer, " \t\r\n\f\v")) // check if that is file in folder
        check_for_file(info, what_check, f, d);
    functions_search(info, what_check);
    info->tab_pos = info->tab_list;
    if (info->tab_list && info->tab_list->next)
        mx_tab_work(info, buffer, position);
}

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
}
