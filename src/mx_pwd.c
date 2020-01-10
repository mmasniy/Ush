#include "ush.h"

static void update_link(char *buff, char *pwd);

int ush_pwd(t_info *info) {
    char *pwd = NULL;
    char *buff = mx_strnew(USH_RL_BUFSIZE);

    pwd = mx_format_pwd(info->env_c[mx_el(info->env_c, "PWD=")]);
    if (info->args[1] == NULL || strcmp(info->args[1], "-L") == 0) {
        printf("%s\n", pwd);
    }
    else if (strcmp(info->args[1], "-P") == 0) {
        if (check_link(pwd)) {
            readlink(pwd, buff, USH_RL_BUFSIZE);
            update_link(buff, pwd);
        }
        else
            printf("pwd = %s\n", pwd);
    }
    free(pwd);
    return 1;
}

static void update_link(char *buff, char *pwd) {
    int i = mx_strlen(pwd);
    int j = mx_strlen(buff);
    char *tmp_p = NULL;
    char *tmp_b = NULL;

    for (; i > 0; i--)
            if (pwd[i] == '/')
                break;
        tmp_p = strndup(pwd, i + 1);
    if (mx_count_substr(buff, "/") == 0){
        tmp_b = mx_strjoin(tmp_p, buff);
        printf("%s\n", tmp_b);
    }
    if (mx_count_substr(buff, "/") == 1) {   
        for (; j > 0; j--)
            if (buff[j] == '/')
                break;
        tmp_b = mx_strjoin(tmp_b, &buff[j + 1]);
        printf("%s%s\n", tmp_p, tmp_b);
    }
    else
        printf("%s\n", buff);
    free(tmp_b);
    free(tmp_p);
}

bool check_link(char *argv) {
    struct stat buff;

    lstat(argv, &buff);
    if (MX_LNK(buff.st_mode))
        return 1;
    return 0;
}
