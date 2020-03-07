#include "../inc/ush.h"

static char *operators2(int type) {
    if (type == 1)
        return ";";
    if (type == 12)
        return "<&";
    if (type == 13)
        return ">&";
    if (type == 16)
        return ">>&";
    if (type == 17)
        return "&>";
    if (type == 18)
        return "&<";
    if (type == 19)
        return "&>>";
    if (type == 20)
        return "&<<";
    return "";
}

static char *operators1(int type) {
    if (type == 2)
        return "&";
    if (type == 3)
        return "|";
    if (type == 4)
        return "<";
    if (type == 5)
        return ">";
    if (type == 7)
        return "||";
    if (type == 6)
        return "&&";
    if (type == 8)
        return "<<";
    if (type == 9)
        return ">>";
    if (type == 11)
        return "<>";
    return operators2(type);
}

void mx_error_message(char *str) {
    mx_printerr(USH);
    mx_printerr(str);
}

void mx_file_not_found(char *filename, t_info *i) {
    mx_printerr("u$h:");
    mx_printerr(MX_NOT_FOUND);
    mx_printerr(filename);
    mx_printerr("\n");
    i->status = 1;
}

void mx_error_mes_tree(int type, t_info *i) {
    char *str;

    mx_printerr(ERROR_PARSE_TREE);
    str = mx_strdup(operators1(type));
    mx_printerr(str);
    mx_printerr("'\n");
    i->status = 1;
    mx_strdel(&str);
}
