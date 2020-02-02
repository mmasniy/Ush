#include "ush.h"
// добавить в либу
void mx_printerr_char(char r);
int mx_strlen_arr(char **argv);
// static void update_link(char *buff, char *pwd);
static char mx_search_bad_flag(char *flags);
static int mx_chack_flag_for_pwd(t_info *i);

int mx_ush_pwd(t_info *info, t_process *p) {
    if (p) {}
    // char *pwd = NULL;
    // char *buff = mx_strnew(USH_RL_BUFSIZE);

    if (mx_chack_flag_for_pwd(info) != -1) {
    	printf("cool\n");
	    // pwd = mx_format_pwd(info->env_c[mx_el(info->env_c, "PWD=")]);
	    // if (info->args[1] == NULL || strcmp(info->args[1], "-L") == 0) {
	    //     printf("%s\n", pwd);
	    // }
	    // else if (strcmp(info->args[1], "-P") == 0) {
	    //     if (check_link(pwd)) {
	    //         readlink(pwd, buff, USH_RL_BUFSIZE);
	    //         update_link(buff, pwd);
	    //     }
	    //     else
	    //         printf("pwd = %s\n", pwd);
	    // }
	    // free(pwd);
	}
    return 1;
}

int mx_strlen_arr(char **argv) {
    int count = 0;

    if (argv) {
        for (int i = 0; argv[i]; i++)
            count++;
        return count;
    }
    return -1;
}

static int mx_chack_flag_for_pwd(t_info *i) {
	char *argv[2]; //= NULL;
	int flag = 0;

	if (mx_strlen_arr(i->args) == 1)
		return 1;
	if (mx_strlen_arr(i->args) < 3) {
		if (mx_strcmp(i->args[1], "-L") == 0
			|| mx_strcmp(i->args[1], "-P") == 0)
			return 1;
		else if (mx_strcmp(i->args[1], "-LP") == 0
			|| mx_strcmp(i->args[1], "-PL") == 0) {
			printf("1\n");
			argv[0] = mx_strdup("pwd");
			argv[1] = mx_strdup("-P");
			printf("2\n");
			mx_del_strarr(&i->args);
			i->args = argv;
			printf("3\n");
		}
		else {
			mx_printerr("pwd: bad option: ");
			mx_printerr_char(mx_search_bad_flag(i->args[1]));
			mx_printerr("\n");
			return -1;
		}
	}
	else if (mx_strlen_arr(i->args) > 2) {
			for (int j = 0; i->args[j]; j++) {
				if (!(mx_strcmp(i->args[1], "-L") == 0 ||
					mx_strcmp(i->args[1], "-P") == 0)) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				argv[0] = mx_strdup("pwd");
				argv[1] = mx_strdup("-P");
				mx_del_strarr(&i->args);
				i->args = argv;
			}
			else {
				mx_printerr("pwd: bad option: ");
				mx_printerr(i->args[1]);
				mx_printerr("\n");
				return -1;
			}
		}
	return 1;
}

void mx_printerr_char(char r) {
	write(2, &r, 1);
}

static char mx_search_bad_flag(char *flags) {
	for (int i = 0; flags[i]; i++)
		if (flags[i] != 'L' || flags[i] != 'P')
			return flags[i];
	return -1;
}

// static void update_link(char *buff, char *pwd) {
//     int i = mx_strlen(pwd);
//     int j = mx_strlen(buff);
//     char *tmp_p = NULL;
//     char *tmp_b = NULL;

//     for (; i > 0; i--)
//             if (pwd[i] == '/')
//                 break;
//         tmp_p = strndup(pwd, i + 1);
//     if (mx_count_substr(buff, "/") == 0){
//         tmp_b = mx_strjoin(tmp_p, buff);
//         printf("%s\n", tmp_b);
//     }
//     if (mx_count_substr(buff, "/") == 1) {   
//         for (; j > 0; j--)
//             if (buff[j] == '/')
//                 break;
//         tmp_b = mx_strjoin(tmp_b, &buff[j + 1]);
//         printf("%s%s\n", tmp_p, tmp_b);
//     }
//     else
//         printf("%s\n", buff);
//     free(tmp_b);
//     free(tmp_p);
// }

bool check_link(char *argv) {
    struct stat buff;

    lstat(argv, &buff);
    if (MX_LNK(buff.st_mode))
        return 1;
    return 0;
}
