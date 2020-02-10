#include "../../inc/ush.h"

int mx_redirection(int type);
void mx_reset_fd(int *fd);

int mx_start_function(t_info *info, char **tree) {
    // mx_del_strarr(info->args);
    info->args = tree;
    if (mx_check_buildin(info, 1) == -1)
        mx_execute_binary_file(info, tree);
    return 0;
}


void mx_printf_strarr(char **str) {
    if (str) {}
    // printf("%s[%s", GRN, RESET);
    // for (int i = 0; str[i]; i++) {
    //      // if (str[i + 1])
    //      //     printf("%s%s%s ",YEL, str[i], RESET);
    //      // else
    //          printf("%s%s%s ",YEL, str[i], RESET);
    // }
    // printf("%s]%s ",GRN, RESET);
}

int mx_execute_binary_file(t_info *info, char **args) {
    char *path = mx_find_in_PATH(info->paths, args[0], 1);
    pid_t pid = fork();

    if (pid == 0) {
        if (execv(path, args) == -1)
            perror(USH);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        perror(USH);
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, /*Повертає управління батьківському процесу*/WUNTRACED); 

        while (!/*Повертає нуль, доки процес триває*/WIFEXITED(status)
            && !/*Повертає не нульове значення, якщо процес завершився помилкою*/WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
    return 0;
}

void mx_reset_fd(int *fd){
    dup2(fd[0], 0);
    dup2(fd[1], 1);
    dup2(fd[2], 2);
}

int mx_tree_run(t_ast *tree, t_info *info, int f) {
    if (tree && tree->type == 10) {
        f = mx_start_function(info, tree->command);
    }
    else if (tree && tree->type == 3)
        mx_run_pipe(tree, info);
    else if (tree && tree->type == 1) {
        f = mx_tree_run(tree->left, info, 0);
        f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && tree->type == 6) {
        if ((f = mx_tree_run(tree->left, info, 0)))
            f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && tree->type == 7) {
        if (!(f = mx_tree_run(tree->left, info, 0)))
            f = mx_tree_run(tree->right, info, 0);
    }
    else if (tree && mx_redirection(tree->type))
        printf("mx_redirection\n");
    return f;
}
static int pipe_2(int p0, int p1, int *flag);

static int pipe3(t_ast *t, t_info *i, int p0, int p1) {
    dup2(p1, 1);
    close(p0);
    return mx_tree_run(t->left, i, 0);
}

int mx_run_pipe(t_ast *tree, t_info *info) {
    int pipes[2];
    pid_t pid[2];
    int flag[2];

    if (pipe(pipes) < 0)
        return 0;
    if ((pid[0] = fork()) == 0) {
        flag[0] = pipe3(tree, info, pipes[0], pipes[1]);
        exit(0);
    }
    if ((pid[1] = fork()) == 0) {
        dup2(pipes[0], 0);
        close(pipes[1]);
        flag[1] = mx_tree_run(tree->right, info,0);
        exit(0);
    }
    if ((pipe_2(pipes[0], pipes[1], flag)) == 0)
        return 0;
    return 1;
}

static int pipe_2(int p0, int p1, int *flag) {
    close(p0);
    close(p1);
    waitpid(-1, 0, 0);
    waitpid(-1, 0, 0);
    if (!flag[0] || !flag[1])
        return 0;
    return 1;
}

int mx_redirection(int type) {
    if (type == 4 || type == 5 || type == 9 || type == 12 || type == 13
        || type == 16 || type == 17 || type == 18 || type == 19 || type == 20)
        return 1;
    return 0;
}

void mx_tok_to_tree(t_tok *tok, t_info *info) {
    t_ast *tree = NULL;
    int fd[3];

    if (!tok)
        return;
    fd[0] = dup(0);
    fd[1] = dup(1);
    fd[2] = dup(2);
    tree = mx_start_tree(tok);
    mx_tree_run(tree, info, 0);
    mx_free_tree(tree);
}


/*
    pipe - открывает для аргуманта fd[2] два дискриптора,
        fd[0] - для чтения, fd[1] - для записи.
    dup - открывает FD
    dup2 - обновляет FD
    fork - создает дочерний процесс
    Интерпретация аргумента pid функцией waitpid зависит от его значения:
    pid == -1 Ожидает завершения любого дочернего процесса. В данном случае функция waitpid эквивалентна функции wait.
    pid > 0 Ожидает завершения процесса с идентификатором, равным pid.
    pid == 0 Ожидает завершения любого дочернего процесса с тем же идентифи- катором группы процессов, что и у вызывающего процесса (группы процессов обсуждаются в разделе 9.4).
    pid < -1 Ожидает завершения любого дочернего процесса с идентификатором группы процессов, совпадающим с pid.
*/
