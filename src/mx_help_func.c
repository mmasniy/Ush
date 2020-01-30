#include "ush.h"

// static void del_sub(t_info *info, int count, int pos);

// void mx_update_pwd(t_info *info) {
//     int pos = 0;
//     char *tmp = NULL;

//     pos = (mx_el(info->env_c, "PWD=") == 0) ? 26 :
//     mx_el(info->env_c, "PWD=");
//     if (mx_count_substr(info->args[1], "../") != 0)
//         del_sub(info, mx_count_substr(info->args[1], "../"), pos);
//     else if (strcmp(info->args[1], "..") == 0 
//         && mx_strcmp(info->env_c[mx_el(info->env_c, "PWD=")], "/") != 0 )
//         del_sub(info, mx_count_substr(info->args[1], ".."), pos);
//     else if (strcmp(info->args[1], "..") != 0) {
//         if (strstr(info->args[1], getenv("HOME")) == 0)
//             tmp = mx_strjoin(info->env_c[pos], "/");
//         else
//             tmp = mx_strjoin(tmp, "PWD=");
//         mx_strdel(&info->env_c[pos]);
//         info->env_c[pos] = mx_strjoin(tmp, info->args[1]);
//     }
//     free(tmp);
// }

// static void del_sub(t_info *info, int count, int pos) {
//     int i = mx_strlen(info->env_c[pos]);
//     char *tmp = NULL;
 
//     tmp = mx_replace_substr(info->args[1], "../", "");
//     mx_strdel(&info->args[1]);
//     info->args[1] = mx_strdup(tmp);
//     mx_strdel(&tmp);
//     for (; count != 0; i--)
//         if (info->env_c[pos][i] == '/')
//             count--;
//     ++i;
//     tmp = mx_strnew(i + 1 + mx_strlen(info->args[1]));
//     tmp = strncpy(tmp, info->env_c[pos], i);
//     mx_strdel(&info->env_c[pos]);
//     if (mx_strlen(tmp) == 4)
//         tmp = mx_strdup("PWD=/");
//     info->env_c[pos] = strdup(tmp);
//     free(tmp);
// }

// int mx_el(char **env_c, char *pwd) {
//     for (int i = 0; env_c[i]; i++)
//         if (mx_strncmp(env_c[i], pwd, mx_strlen(pwd)) == 0)
//             return i;
//     return 0;
// }
