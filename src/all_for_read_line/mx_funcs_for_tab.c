#include "../../inc/ush.h"

void mx_replace_special_symbols(t_info *info) {
    char tmp_symbols[2] = "\0\0";
    char tmp_replace_symbols[3] = "^\0\0";
    char symbols[8] = {'\a', '\b', '\t', '\n', '\v', '\f', '\r', 27};
    char replace_symbols[8] = {'G', 'H', 'I', 'J', 'K', 'L', 'M', '['};

    for (t_history *tmp = info->tab_list; tmp; tmp = tmp->next) {
        for (int i = 0; i < 8; i++) {
            tmp_symbols[0] = symbols[i];
            tmp_replace_symbols[1] = replace_symbols[i];
            mx_del_and_set(&(tmp->data), mx_replace_substr(tmp->data,
                           tmp_symbols, tmp_replace_symbols));
        }
        mx_del_and_set(&(tmp->data), mx_replace_substr(tmp->data, " ", "\\ "));
    }
}
