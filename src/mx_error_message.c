#include "../inc/ush.h"

static char *operators1(t_ast *t);
static char *operators2(t_ast *t);

void mx_error_message(char *str) {
    mx_printerr(RED);
    mx_printerr(USH);
    mx_printerr(str);
    mx_printerr(RESET);
}

void mx_error_mes_tree(t_ast *t) {
    char *str;

    mx_printerr(RED);
    mx_printerr(ERROR_PARSE_TREE);
    str = mx_strdup(operators1(t));
    // if (mx_strcmp(str, "") != 0)
        mx_printerr(str);
    mx_printerr("'\n");
    mx_printerr(RESET);
}

static char *operators1(t_ast *t) {
    if (t->type == 2)
        return "&";
    if (t->type == 3)
        return "|";
    if (t->type == 4)
        return "<";
    if (t->type == 5)
        return ">";
    if (t->type == 6)
        return "||";
    if (t->type == 7)
        return "&&";
    if (t->type == 8)
        return "<<";
    if (t->type == 9)
        return ">>";
    if (t->type == 11)
        return "<>";
    return operators2(t);
}

static char *operators2(t_ast *t) {
    if (t->type == 12)
        return "<&";
    if (t->type == 13)
        return ">&";
    if (t->type == 16)
        return ">>&";
    if (t->type == 17)
        return "&>";
    if (t->type == 18)
        return "&<";
    if (t->type == 19)
        return "&>>";
    if (t->type == 20)
        return "&<<";
    return "";
}
