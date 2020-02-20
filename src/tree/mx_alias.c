#include "../../inc/ush.h"

// int mx_search_als(char *cmd, t_alias *all_als) {
// 	t_alias *als = all_als;

// 	while (als->next) {
// 		if (mx_strcmp(cmd[0], als->name) == 0)
// 			break
// 		als = als->next;
// 	}
// }

void mx_get_value_als(t_alias *a, char **alias, int i) { 
	t_alias *als = a;
	char **temp_als = mx_strsplit(*alias, ' ');
	int size_tmp = 0;
	// int flag = 0;

	// char *tmp_name[4] = {"m","mk", "l", NULL};
	// char *tmp_value[4] = {"mk","make && ./ush", "ls -l", NULL};
	// int j = 0;
	// mx_print_strarr(temp_als, "\n");
	while (als && temp_als[i]) {
		// j = 0;
		while (als && als->next) {//(tmp_name[j]) { //als->next
			if (mx_strcmp(temp_als[i], als->name) == 0) { // tmp_name[j]
				// printf("do temp_als = %s\n", temp_als[i]);
				mx_strdel(&temp_als[i]);
				// printf("tmp_value[j] = %s\n", tmp_value[j]);
				temp_als[i] = mx_strdup(als->value); //tmp_value[j]
				// printf("temp_als = %s\n", temp_als[i]);
				// flag++;
				break;
			}
			als = als->next; //j++;
		}
		i++;
	}
	size_tmp = mx_allarr_size(temp_als);
	mx_strdel(alias);
	*alias = mx_strarr_to_str(temp_als, size_tmp, 0);
	mx_del_strarr(&temp_als);
	// printf("---------------------------\nalias = %s\n------------------------\n", *alias);
	// if (flag != 0)
	// 	mx_get_value_als(alias);
	// return *alias;
}

static void to_name(char **alias) {
	char *tmp_rep = mx_replace_substr(*alias, "alias", " ");
	char *without_space = mx_del_extra_spaces(tmp_rep);

	mx_strdel(alias);
	mx_strdel(&tmp_rep);
	*alias = mx_strdup(without_space);
	mx_strdel(&without_space);
}

char *mx_get_name_als(char **alias, int count) {
	char *name;
	char *tmp;

	to_name(alias);
	while ((*alias)[count] && (*alias)[count] != '=')
		count++;
	name = mx_strndup(*alias, count);
	tmp = mx_strndup(((*alias) + count + 2),
		mx_strlen((*alias)) - count - 1);
	mx_strdel(alias);
	*alias = mx_strndup(tmp, mx_strlen(tmp) - 1);
	mx_strdel(&tmp);
	return name;
}

t_alias *mx_create_als(t_alias *als, char *alias) {
	t_alias *start = (t_alias *)malloc(sizeof(t_alias));
	int count = 0;
	printf("2\n");
	start->name = mx_get_name_als(&alias, count);
	printf("start->name = %s\n", start->name);
	mx_get_value_als(als, &alias, 0);
	start->value = mx_strdup(alias);
	printf("start->value = %s\n", start->value);
	start->next = NULL;
	return start;
}

void mx_add_als(t_info *i, char *alias) {
	printf("1\n");
	t_alias *new = mx_create_als(i->alias, alias);
	
	if (i->alias) {
		t_alias *tmp = i->alias;

		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;	
	}
	else
		i->alias = new;
}

void mx_print_lias_alias(t_info *i) {
	while (i->alias->next) {
		if (i->alias->name)
			printf("%s[%s]=\"",GRN, i->alias->name);
		if (i->alias->value)
			printf("%s\"%s", i->alias->value, RESET);
		printf("\n");
		i->alias = i->alias->next;
	}
}

void mx_check_alias(t_ast *t, t_info *i, int a) {
    int count = 0;
    char *alias;
    printf("tut\n");
    while (t->command[a]) {
        count += mx_count_substr(t->command[a], "alias");
        a++;
    }
    if (count == 1) {
    	printf("i tut\n");
    	alias = mx_strarr_to_str(t->command, mx_allarr_size(t->command), 0);
    	printf("%s\n", alias);
    	mx_add_als(i, alias);
    	printf("tut toje\n");
    	mx_print_lias_alias(i);
    }
    else if (count > 1){
        //print errors alias
        printf("errors alias\n");
    }
}

//void mx_als(t_alias **als) {
//     (*als) = (t_alias *)malloc(sizeof(t_alias));
//     (*als)->name = mx_strdup("go");
//     (*als)->value = mx_strdup("clang");
//     (*als)->next = NULL;
//     t_alias *als2 = (t_alias *)malloc(sizeof(t_alias));
//     als2->name = mx_strdup("gs");
//     als2->value = mx_strdup("go -std=c11");
//     als2->next = NULL;
//     (*als)->next = als2;
//     t_alias *als3 = (t_alias *)malloc(sizeof(t_alias));
//     als3->name = mx_strdup("gsw");
//     als3->value = mx_strdup("gs -Wextra");
//     als3->next = NULL;
//     (*als)->next->next = als3;
// }
