#include "ush.h"

int mx_test(t_info *info, t_process *p) {
    if (info && p) {}

    // char str[] = "cat rw.c | cat";
    char *firstArgs[] = {"cat Makefile", NULL};
    // char *secondArgs[] = {"cat",0};
    // int fds[2];

    // pipe(fds);

    pid_t pid = fork();

    if (pid == 0) {
        execvp("/bin/cat", firstArgs);
        // write(fds[1], "Hello!\0", 7);
    }
    else {
        // pid = fork();
        // execv()
        // char buf[7];
        // sleep(5);
        // read(fds[0], buf, 7);
        // write(fds[1], buf, 7);
            // printf("Child2\n");
    }
    // printf("Parent\n");
    return 0;
}
// pipe(fds);

// int fd = open("_.txt", O_RDWR | O_APPEND | O_CREAT, S_IWRITE | S_IREAD);
// if (fd == -1) {
//     mx_printerr(strerror(errno));
//     return 0;
// }

// write(fd, "c", 1);
// pid_t pid = fork();

// if (pid == 0) {
//     write(fd, "a", 1);
// }
// else {
//     write(fd, "b", 1);
// }



// pid_t pid = fork();

// if (pid == 0) {
//     write(fds[1], "Hello!\0", 7);
//     // while(1) {
//     //     sleep(1);
//     //     printf("CHILD!\n");
//     // }
//     // execv("/bin/cat", firstArgs);
//     // slee(3);
//     // printf("BB\n");
// }
// else {
//     char buf[7];
//     read(fds[0], buf, 7);
//     write(fds[1], buf, 7);
//     // printf("AA\n");
//     // wait(0);
//     // printf("AB\n");
//     // pid = fork();

//     // if (pid == 0) {

//     //     execv("/bin/cat", secondArgs);
//     // }
//     // else {
//     //     close(fds[1]);
//     //     close(fds[0]);
//     // }
// }
