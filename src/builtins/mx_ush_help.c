#include "../../inc/ush.h"

int mx_ush_help(t_info *info) {
    (void)info;
    printf("\"pwd\", \"cd\", \"help\", \"exit\", \"history\", \"env\"");
    printf(", \"unset\", \"export\", \"which\", \"echo\", \"jobs\", \"fg\"");
    printf(", \"custom\", \"true\", \"false\", \"return\", \"set\"\n");
    printf("Use the man command for information on other programs.\n");
    return 1;
}
