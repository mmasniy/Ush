#include "../../inc/ush.h"

int mx_ush_help(t_info *info) {
    (void)info;
    printf("\"pwd\",\"cd\",\"help\",\"exit\",\"history\",\"env\" \
    ,\"unset\",\"export\",\"which\",\"echo\",\"jobs\",\"fg\",\"test\"\n");
    printf("Use the man command for information on other programs.\n");
    return 1;
}
