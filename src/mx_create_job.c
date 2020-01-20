#include "ush.h"

t_job *mx_create_job(t_info *info, char **args) {
    int index = 0;
    t_job *new_job = NULL;
    new_job = (t_job *) malloc(sizeof(t_job));
    t_process *first_p = NULL;

//    t_process *second_p = NULL;
    first_p = (t_process *) malloc(sizeof(t_process));
//    second_p = (t_process *) malloc(sizeof(t_process));
    first_p->command = mx_strdup(args[0]);
    first_p->argv = mx_arr_copy(args);
    first_p->foreground = 1;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "&") == 0)
            first_p->foreground = 0;
    }
    first_p->pipe = 0;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "&") == 0)
            first_p->foreground = 0;
    }
    first_p->type = index;

    for (int i = 0; i < info->num_of_func; i++) {
       if (strcmp(info->args[0], info->builtin_str[i]) == 0) {
            first_p->type = 1;
            break;
       }
    }
    first_p->next = NULL;
    new_job->first_process = first_p;
    new_job->command = args[0];
    if (first_p->foreground)
        new_job->foreground = FOREGROUND;
    else
        new_job->foreground = BACKGROUND;

    //  new_job->pgid = getpid();
    new_job->pgid = -1;
    new_job->stdin = 0;
    new_job->stdout = 1;
    new_job->stderr = 2;
    return new_job;
}
