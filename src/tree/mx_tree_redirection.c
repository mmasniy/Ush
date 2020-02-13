#include "../../inc/ush.h"

int mx_redirection(int type) {
    if (type == 4 || type == 5 || type == 9 || type == 12 || type == 13
        || type == 16 || type == 17 || type == 18 || type == 19 || type == 20)
        return 1;
    return 0;
}

int mx_create_file(t_ast *t) {
    if (t->type == 5 || t->type == 17)
        return (open(t->right->command[0], O_WRONLY | O_CREAT | O_TRUNC, 0600));
    if (t->type == 9 || t->type == 19)
        return (open(t->right->command[0], O_WRONLY | O_APPEND | O_CREAT, 0600));
    if (t->type == 4)
        return (open (t->command[0], O_RDONLY | O_CREAT, 0600));
    return -1;
}

