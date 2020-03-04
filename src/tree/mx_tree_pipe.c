#include "../../inc/ush.h"

static int pipe_2(int p0, int p1, int *flag) {
    close(p0);
    close(p1);
    waitpid(-1, 0, 0);
    waitpid(-1, 0, 0);
    if (!flag[0] || !flag[1])
        return 0;
    return 1;
}

static int pipe3(t_ast *t, t_info *i, int p0, int p1) {
    dup2(p1, 1);
    close(p0);
    return mx_tree_run(t->left, i, 0);
}

int mx_run_pipe(t_ast *tree, t_info *i) {
    int pipes[2];
    pid_t pid[2];
    int flag[2];

    if (pipe(pipes) < 0)
        return 0;
    if ((pid[0] = fork()) == 0) {
        flag[0] = pipe3(tree, i, pipes[0], pipes[1]);
        exit(0);
    }
    if ((pid[1] = fork()) == 0) {
        dup2(pipes[0], 0);
        close(pipes[1]);
        flag[1] = mx_tree_run(tree->right, i, 0);
        exit(0);
    }
    if ((pipe_2(pipes[0], pipes[1], flag)) == 0)
        return 0;
    return 1;
}
