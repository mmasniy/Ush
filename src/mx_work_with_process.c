#include "../inc/ush.h"

/*
// A job is a pipeline of processes.
typedef struct  s_job {
    int     job_id;                 //number in jobs control
    int     mark_job_id;            // " ", "-", "+"   "+" - last addmx_print_jobsed job, "-" - prev added job;
    char    *command;              // command line, used for messages
    t_process *first_process;     // list of processes in this job
    pid_t   pgid;                 // process group ID
    int     exit_code;
    int     mode;                  // foreground or background execution
    char    notified;              // true if user told about stopped job
    struct  termios tmodes;      // saved terminal modes
    int     stdin;  // standard i/o channels
    int     stdout;  // standard i/o channels
    int     stderr;  // standard i/o channels
//    struct  s_job *next;           // next active job
} t_job;
*/

int mx_get_next_job_id(t_info *info) {
    for (int i = 1; i < info->max_number_job; i++) {
        if (info->jobs[i] == NULL) {
            info->max_number_job++;
            return i;
        }
    }
    return -1;
}

int mx_insert_job(t_info *info, t_job *job) {
    int id = mx_get_next_job_id(info);

    if (id < 0) {
        return -1;
    }
    job->job_id = id;
    job->mark_job_id = 43;
    info->jobs[id] = job;
    // write(1, "job insert\n", strlen("job insert\n"));
    return id;
}

void mx_remove_job(t_info *info, int id) {
    if (id > JOBS_NUMBER || info->jobs[id] == NULL)
        exit(-1);
//    mx_destroy_jobs(info, id);
    if (id == info->max_number_job - 1)
        info->max_number_job--;
    info->jobs[id] = NULL;
    // write(1, "job remove\n", strlen("job remove\n"));
}

void mx_destroy_jobs(t_info *info, int id) {
    t_process *proc;

    for (proc = info->jobs[id]->first_process; proc != NULL; proc = proc->next) {
        free(proc->argv);
        free(proc->arg_command);
        free(proc->command);
    }
    info->jobs[id]->first_process = NULL;
    free(info->jobs[id]->command);
}

void mx_print_process_in_job(t_info *info, int id) {
    if (0) {
        t_process *p;

        if (id > JOBS_NUMBER || info->jobs[id] == NULL) {
            mx_printstr("error job_id");
            mx_printstr("\n");
        } else {
            printf("[%d]", id);
            for (p = info->jobs[id]->first_process; p != NULL; p = p->next) {
                printf(" %d", p->pid);
            }
            printf("\n");
        }
    }
}


int mx_get_proc_count(t_info *info, int job_id, int filter) {
    t_process *proc;
    int count = 0;

    if (job_id > JOBS_NUMBER || info->jobs[job_id] == NULL) {
        return -1;
    }
    for (proc = info->jobs[job_id]->first_process; proc != NULL; proc = proc->next) {
        if (filter == PROC_FILTER_ALL ||
            (filter == PROC_FILTER_DONE && proc->status == STATUS_DONE) ||
            (filter == PROC_FILTER_REMAINING && proc->status != STATUS_DONE)) {
            count++;
        }
    }
    return count;
}


int mx_wait_job(t_info *info, int id) {
    int proc_count = mx_get_proc_count(info, id, PROC_FILTER_REMAINING);
    int wait_pid = -1;
    int wait_count = 1;
    int status = 0;

    if (id > JOBS_NUMBER || info->jobs[id] == NULL) {
        return -1;
    }
  //  wait_pid = waitpid(-info->jobs[id]->pgid, &status, WUNTRACED);
    while (wait_count < proc_count) {
        wait_pid = waitpid(-info->jobs[id]->pgid, &status, WUNTRACED);
        wait_count++;
        if (WIFEXITED(status)) {
            mx_set_process_status(info, wait_pid, STATUS_DONE);
        } else if (WIFSIGNALED(status)) {
            mx_set_process_status(info, wait_pid, STATUS_TERMINATED);
        } else if (WSTOPSIG(status)) {
            status = -1;
            mx_set_process_status(info, wait_pid, STATUS_SUSPENDED);
            if (wait_count == proc_count) {
                mx_print_job_status(info, id);
            }
        }
    }
    return status;
}

int mx_wait_pid(t_info *info, int pid) {
    int status = 0;

    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
        mx_set_process_status(info, pid, STATUS_DONE);
    } else if (WIFSIGNALED(status)) {
        mx_set_process_status(info, pid, STATUS_TERMINATED);
    } else if (WSTOPSIG(status)) {
        status = -1;
        mx_set_process_status(info, pid, STATUS_SUSPENDED);
    }
    return status;
}


void mx_set_process_status(t_info *info, int pid, int status) {
    int i;
    t_process *proc;

    for (i = 1; i < info->max_number_job; i++) {
        if (info->jobs[i] == NULL) {
            continue;
        }
        for (proc = info->jobs[i]->first_process; proc != NULL; proc = proc->next) {
            if (proc->pid == pid) {
                proc->status = status;
            }
        }
    }
}

int mx_print_job_status(t_info *info, int id) {
    if (0) {
        t_process *proc;
        const char* status[] = {
                "running",
                "done",
                "suspended",
                "continued",
                "terminated"
        };

        if (id > JOBS_NUMBER || info->jobs[id] == NULL)
            return -1;
        printf("[%d] %c", id, info->jobs[id]->mark_job_id);
        for (proc = info->jobs[id]->first_process; proc != NULL; proc = proc->next) {
            printf("\t%d\t%s\t%s\t%s", proc->pid, status[proc->status], proc->command, proc->argv[1]);
            if (proc->next != NULL) {
                printf("|\n");
            } else {
                printf("\n");
            }
        }
    }
    return 0;
}

int mx_is_job_completed(t_info *info, int id) {
    t_process *proc;

    if (id > JOBS_NUMBER || info->jobs[id] == NULL) {
        return 0;
    }
    for (proc = info->jobs[id]->first_process; proc != NULL; proc = proc->next) {
        if (proc->status != STATUS_DONE) {
            return 0;
        }
    }

    return 1;
}

void mx_check_jobs(t_info *info) {
    int status;
    pid_t pid;  //!!

    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        if (WIFEXITED(status)) {
            mx_set_process_status(info, pid, STATUS_DONE);
        } else if (WIFSTOPPED(status)) {
            mx_set_process_status(info, pid, STATUS_SUSPENDED);
        } else if (WIFCONTINUED(status)) {
            mx_set_process_status(info, pid, STATUS_CONTINUED);
        }
        int job_id = mx_get_job_id_by_pid(info, pid);
        if (job_id > 0 && mx_is_job_completed(info, job_id)) {
            mx_print_job_status(info, job_id);
            mx_remove_job(info, job_id);
        }
        }
    }

int mx_get_job_id_by_pid(t_info *info, int pid) {
    int i;
    t_process *proc;

    for (i = 1; i <= info->max_number_job; i++) {
        if (info->jobs[i] != NULL) {
            for (proc = info->jobs[i]->first_process; proc != NULL; proc = proc->next) {
                if (proc->pid == pid) {
                    return i;
                }
            }
        }
    }
    return -1;
}

int mx_get_pgid_by_job_id(t_info *info, int job_id) {
    if (job_id > JOBS_NUMBER || info->jobs[job_id] == NULL)
        return -1;

    return info->jobs[job_id]->pgid;
}

/*

int mx_get_recent_job(t_info *info)

}
*/

int mx_set_job_status(t_info *info, int job_id, int status) {
    t_process *proc;

    if (job_id > JOBS_NUMBER || info->jobs[job_id] == NULL) {
        return -1;
    }
    for (proc = info->jobs[job_id]->first_process; proc != NULL; proc = proc->next) {
        if (proc->status != STATUS_DONE) {
            proc->status = status;
        }
    }
    return 0;
}
