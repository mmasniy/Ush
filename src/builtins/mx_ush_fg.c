#include "../../inc/ush.h"

int mx_fg(t_info *info) {
    if (info) {}
// ////    if  !job_id
//     pid_t pid = 0;
//     int job_id = 0;

//     if (p->argv[1]) {
//         //  if (p->arg_command[0] == '%') {
//         job_id = atoi((p->argv[1]));
// //        else
// //            job_id = mx_get_recent_job(info); //most recently placed in the background, find '+'
//         //}
//     }
//     //       else {
// //            pid = atoi((p->arg_command[1]));
//     pid = mx_get_pgid_by_job_id(info, job_id);
// //    if (kill(-pid, SIGCONT) < 0) {
//     if (kill(-0, SIGCONT) < 0) {
//         mx_printerr("fg: job not found: ");
//         mx_printerr(mx_itoa(pid));
//         mx_printerr("\n");
//         return -1;
//     }

//     tcsetpgrp(0, pid);
//     if (job_id > 0) {
//         mx_set_process_status(info, job_id, STATUS_CONTINUED);
//         mx_print_job_status(info, job_id);
//         if (mx_wait_job(info, job_id) >= 0)
//             mx_remove_job(info, job_id);
//     }
//     else
//         mx_wait_pid(info, pid);
//     signal(SIGTTOU, SIG_IGN);  //Запись в управляющий терминал процессом из группы процессов фонового режима.
//     tcsetpgrp(0, getpid());
//     signal(SIGTTOU, SIG_DFL);  //
    return 0;
}
