#include "../../inc/ush.h"

t_export *mx_search_key_in_list(t_export *list, char *key) {
    for (t_export *tmp = list; tmp; tmp = tmp->next) {
        if (strcmp(tmp->key, key) == 0)
            return tmp;
    }
    return NULL;
}

t_export *mx_create_new_export(char *key, char *value) {
    t_export *new_elem = (t_export *)malloc(sizeof(t_export));

    if (new_elem) {
    	if (key)
        	new_elem->key = mx_strdup(key);
        else
        	new_elem->key = NULL;
        if (value)
        	new_elem->value = mx_strdup(value);
        else
        	new_elem->value = mx_strdup("");
        new_elem->next = NULL;
    }
    return new_elem;
}

void mx_push_export_back(t_export **list, char *key, char *value) {
    t_export *new_elem = NULL;

    if (list && (new_elem = mx_create_new_export(key, value)) != NULL) {
        if (*list) {
            t_export *temp = *list;
            while (temp->next)
                temp = temp->next;
            temp->next = new_elem;
        }
        else {
            *list = new_elem;
        }
    }
}

void mx_pop_export_front(t_export **head) {
    t_export *temp = NULL;

    if (head && *head) {
        temp = *head;
        if (malloc_size(temp->key))
            mx_strdel(&(temp->key));
        if (malloc_size(temp->value))
            mx_strdel(&(temp->value));
        *head = temp->next;
        free(temp);
    }
}

bool mx_check_is_continue(char *craft, int *pos, char **new_line, char **chk) {
    if (mx_is_quotes(craft, *pos) == '\'' || !craft[*pos + 1]
        || mx_is_allow(craft[*pos + 1]) == 0) {
        mx_del_and_set(new_line, mx_strjoin(*new_line, "$"));
        (*pos)++;
        mx_strdel(chk);
        return 1;
    }
    return 0;
}
