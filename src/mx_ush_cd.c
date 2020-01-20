#include "ush.h"

static char *f_oldpwd(char *pwd);
static void update_env(t_info *st, char *pwd, char *old_pwd);

int mx_ush_cd(t_info *st, t_process *p) {
    if (p) {}
    DIR *dp;
    if ((dp = opendir(st->args[1])) == NULL) {
        perror("ush");
        return 1;
    }
    st->OLDPWD = mx_strdup(st->env_c[mx_el(st->env_c, "PWD=")]);
    if ((!st->args[1] || strcmp(st->args[1], "~") == 0)) {
        st->PWD = mx_strdup(getenv("HOME"));
        chdir(st->PWD);
    }
    else if (strcmp(st->args[1], "-") == 0) {
        st->PWD = f_oldpwd(st->env_c[mx_el(st->env_c, "OLDPWD=")]);
        chdir(st->PWD);
    }
    else if ((chdir(st->args[1])) == 0) {
        mx_update_pwd(st);
        st->PWD = mx_format_pwd(st->env_c[mx_el(st->env_c, "PWD=")]);
    }
    update_env(st, st->PWD, st->OLDPWD);
    mx_strdel(&st->PWD);
    mx_strdel(&st->OLDPWD);
    return 1;
}

static char *f_oldpwd(char *pwd) {
    char *new_pwd = mx_strnew(mx_strlen(pwd) - 7);

    new_pwd = mx_memmove(new_pwd, &pwd[7], mx_strlen(pwd) - 7);
    return new_pwd;
}

static void update_env(t_info *st, char *pwd, char *old_pwd) {
    int O_p = mx_el(st->env_c, "OLDPWD=");
    int P_p = mx_el(st->env_c, "PWD=");
    char *O_temp;
    char *P_temp;

    O_temp = mx_strjoin("OLDPWD=", &old_pwd[4]);
    P_temp = mx_strjoin("PWD=", pwd);
    free(st->env_c[O_p]);
    free(st->env_c[P_p]);
    st->env_c[O_p] = mx_strdup(O_temp);
    st->env_c[P_p] = mx_strdup(P_temp);
    free(O_temp);
    free(P_temp);
}

char *mx_format_pwd(char *pwd) {
    char *new_pwd = mx_strnew(mx_strlen(pwd) - 4);

    new_pwd = mx_memmove(new_pwd, &pwd[4], mx_strlen(pwd) - 4);
    return new_pwd;
}
