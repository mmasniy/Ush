#include "../../inc/ush.h"

char *mx_get_value_als(t_alias *a, char **alias, int i) { 
	t_alias *als = NULL;
	char **temp_als = mx_strsplit(*alias, ' ');

	while (temp_als[i]) {
		als = a;
		while (als) {
			if (mx_strcmp(temp_als[i], als->name) == 0) {
				mx_strdel(&temp_als[i]);
				temp_als[i] = mx_strdup(als->value);
				break;
			}
			als = als->next;
		}
		i++;
	}
	mx_strdel(alias);
	*alias = mx_strarr_to_str(temp_als, 0);
	mx_del_strarr(&temp_als);
	return *alias;
}


static int check_name_alias(t_alias *als, char *name, char *value) {
	t_alias *tmp = als;

	while (tmp) {
		if (mx_strcmp(tmp->name, name) == 0){
			printf("Naideno\n");
			mx_strdel(&(tmp->value));
			tmp->value = mx_strdup(value);
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}


t_alias *mx_create_als(t_alias **als, char *alias, t_info *i) {
	char *name = mx_get_name_als(&alias, i, 0);
	char *value = mx_get_value_als(*als, &alias, 0);
	t_alias *start = NULL;

	if (check_name_alias(i->alias, name, value) == 0) {
		start = (t_alias *)malloc(sizeof(t_alias));
		start->name = mx_strdup(name);
		start->value = mx_strdup(value);
		start->next = NULL;
	}
	return start;
}

void mx_add_als(t_alias **als, char *alias, t_info *i) {
	t_alias *new = mx_create_als(als, alias, i);

	if (*als) {
		t_alias *tmp = *als;

		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*als = new;
}

static void add_als(t_ast *t, t_info *i, char *alias, int a) {
	int count = 0;
    t_alias *als = i->alias;

    while (t->command[a]) {
        count += mx_count_substr(t->command[a], "alias");
        a++;
    }
    if (count == 1) {
    	alias = mx_strarr_to_str(t->command, 0);
    	mx_add_als(&als, alias, i);
    }
    mx_strdel(&alias);
    i->alias = als;
}

static void to_name(char **alias) {
	char *tmp_rep = mx_replace_substr(*alias, "alias", " ");
	char *without_space = mx_del_extra_spaces(tmp_rep);

	mx_strdel(alias);
	mx_strdel(&tmp_rep);
	*alias = mx_strdup(without_space);
	mx_strdel(&without_space);
}


char *mx_get_name_als(char **alias, t_info *i, int count) {
	char *name;
	char *tmp;

	to_name(alias);
	while ((*alias)[count] && (*alias)[count] != '=')
		count++;
	name = mx_strndup(*alias, count);
	// check_name_alias(i, name);
	tmp = mx_strndup(((*alias) + count + 2),
		mx_strlen((*alias)) - count - 1);
	mx_strdel(alias);
	*alias = mx_strndup(tmp, mx_strlen(tmp) - 1);
	mx_strdel(&tmp);
	return name;
}

void mx_add_and_check_alias(t_ast *t, t_info *i, int a) {
    add_als(t, i, NULL, 0);
    // while ()
}

void mx_print_lias_alias(t_alias *als) {
	t_alias *tmp = als;

	while (tmp) {
		if (tmp->name)
			printf("%s[%s]=\"",GRN, tmp->name);
		if (tmp->value)
			printf("%s\"%s", tmp->value, RESET);
		printf("\n");
		tmp = tmp->next;
	}
}
// alias go="clang"
// alias gs="go -std=11"
// alias ggwp="gs -Werror"
