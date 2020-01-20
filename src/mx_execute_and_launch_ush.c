#include "ush.h"

int mx_ush_execute(t_info *info, t_job *job) {
//    pid_t wpid;
    int status = 1;
    int shell_terminal;
    int shell_is_interactive;
    int job_id = 0;  // for job contoll

    setbuf(stdout, NULL); /* установить небуферизованный режим */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty(shell_terminal);
//    tcsetattr(STDIN_FILENO, TCSANOW, &info->t_original);
    t_process *p;
//    pid_t pid;
    int mypipe[2];
    int infile = job->stdin;
    int outfile = 1;
    int errfile = 2;

//  job control
    // mx_check_jobs(info);
//    if (job->first_process->type == 0) {
    job_id = mx_get_next_job_id(info);
    mx_insert_job(info, job);            //insert process to job control
//  }
    for (p = info->jobs[job_id]->first_process; p; p = p->next) {  //list of process in job
        if (p->next) {
            if (pipe(mypipe) < 0) {
                perror("pipe");
                exit(1);
            }
            outfile = mypipe[1];
        } 
        else
            outfile = job->stdout;
        status = mx_launch_process(info, job_id, p, infile, outfile, errfile);
    }

    if (job->foreground == FOREGROUND) {
        mx_print_process_in_job(info, job->job_id);
        mx_remove_job(info, job->job_id);
    } 
    else if (job->foreground == BACKGROUND) {
        mx_print_process_in_job(info, job->job_id);
    }
    return status;
}

int mx_launch_process(t_info *info, int job_id, t_process *p, int infile, int outfile, int errfile) {
    pid_t pgid = info->jobs[job_id]->pgid;

    p->status = STATUS_RUNNING;

    for (int i = 0; i < info->num_of_func; i++) {
        if (strcmp(info->args[0], info->builtin_str[i]) == 0) {
            return (p->exit_code = (*info->builtin_func[i])(info, p));
        }
    }
    pid_t child = fork();
    if ((child < 0)) {  //The fork failed.  Report failure.
        perror("error fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0) {
        // printf("One\n");
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);
        //redirection
        p->pid = getpid();
        if (pgid > 0) {
            setpgid(0, pgid);
        }
        else {
            pgid = p->pid;
            setpgid(0, pgid);
        }
        if (infile != STDIN_FILENO) {
            dup2(infile, STDIN_FILENO);
            close(infile);
        }
        if (outfile != STDOUT_FILENO) {
            dup2(outfile, STDOUT_FILENO);
            close(outfile);
        }
        if (errfile != STDERR_FILENO) {
            dup2(errfile, STDERR_FILENO);
            close(errfile);
        }
        if (execvp(p->argv[0], p->argv) < 0) {
            mx_printerr("u$h: command not found: ");
            mx_printerr(p->argv[0]);
            mx_printerr("\n");
            // perror("execvp");
            _exit(0);
        }
        if (WIFEXITED(p->status))
            p->exit_code = WEXITSTATUS(p->status);
        return (p->exit_code);
        exit(EXIT_SUCCESS);
    }
    else {    //his is the parent process.  Wait for the child to complete.
        // printf("Two\n");
        p->pid = child;
        if (pgid > 0) {
            setpgid(child, pgid);
        }
        else {
            pgid = p->pid;
            setpgid(child, pgid);
        }
        if (info->jobs[job_id]->foreground == FOREGROUND) {
            int status;
            pid_t wpid;
            tcsetpgrp(0, info->jobs[job_id]->pgid);
            wpid = waitpid(pgid, &status, /*Повертає управління батьківському процесу*/WUNTRACED);
            while (!/*Повертає нуль, доки процес триває*/WIFEXITED(status) 
                && !/*Повертає не нульове значення, якщо процес завершився помилкою*/WIFSIGNALED(status)) {
                wpid = waitpid(pgid, &status, WUNTRACED);
            }

            mx_wait_job(info, job_id);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, getpid());
            signal(SIGTTOU, SIG_DFL);
            mx_print_job_status(info, p->pid);
        }
    }
    return (p->exit_code);
}

// int mx_ush_execute(t_info *info, t_job *job) {
//  if (job) {}
//  int i;

//  if (info->args[0] == NULL)
//     return 1;
//  for (i = 0; i < info->num_of_func; i++) {
//     if (strcmp(info->args[0], info->builtin_str[i]) == 0) {
//              return (*info->builtin_func[i])(info);
//     }
//  }
//  //mx_del_strarr(&info->args);
//  return mx_ush_launch(info);
// }

// int mx_ush_launch(t_info *info) {
//  pid_t pid;
//  pid_t wpid;
//  int status;

//  pid = fork();
//  if (pid == 0) {
//      // Child process
//      if (execvp(info->args[0], info->args) == -1) {
//          mx_printerr(RED);
//          perror(USH);
//          mx_printerr(RESET);
//      }
//      exit(EXIT_FAILURE);
//  }
//  else if (pid < 0) {
//      // Error forking
//      mx_printerr(RED);
//      perror(USH);
//      mx_printerr(RESET);
//  } 
//  else {
//  // Parent process
//      wpid = waitpid(pid, &status, /*Повертає управління батьківському процесу*/WUNTRACED);
//      while (!/*Повертає нуль, доки процес триває*/WIFEXITED(status) 
//      && !/*Повертає не нульове значення, якщо процес завершився помилкою*/WIFSIGNALED(status)) {
//          wpid = waitpid(pid, &status, WUNTRACED);
//      }
//  }
//  return 1;
// }
