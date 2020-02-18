#include "../../inc/ush.h"

int mx_jobs(t_info *info, t_process *p) {
    for (int i = 0; i < JOBS_NUMBER; i++) {
        if (info->jobs[i] != NULL) {
            mx_print_job_status(info, i);
        }
    }
    return (p->exit_code);
}
