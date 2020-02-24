#include "../../inc/ush.h"

void mx_save_all_history(t_info *info) {
    FILE *f = fopen(".history_ush.txt", "w+");

    for (t_history *tmp = info->history_pack->history; tmp; tmp = tmp->next) {
        fprintf(f, "%s\n", tmp->data);
    }
    fflush(f);
    fclose(f);
}

void mx_check_history(t_info *info, char *line) {
    if (mx_strlen(line)) {
        free(info->history_pack->history->data);
        info->history_pack->history->data = mx_strdup(line);
    }
    else
        mx_pop_history_front(&info->history_pack->history);
    if (info->history_pack->total_num < 250)
        (info->history_pack->total_num)++;
    else
        mx_pop_history_back(&info->history_pack->history);
}
