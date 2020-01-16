#include "ush.h"

t_history *mx_create_new_history(char *data) {
	t_history *new_history = (t_history *)malloc(sizeof(t_history));

	if (new_history) {
		if (data)
			new_history->data = mx_strdup(data);
		else
			new_history->data = NULL;
		new_history->next = NULL;
		new_history->prev = NULL;
	}
	return new_history;
}

void mx_push_history_front(t_history **lst, void *data) {
	t_history *new_history_elem = NULL;

	if (lst && (new_history_elem = mx_create_new_history(data)) != NULL) {
		new_history_elem->next = *lst;
		if (*lst) {
			(*lst)->prev = new_history_elem;
			*lst = new_history_elem;
		}
		else
			*lst = new_history_elem;
	}
}

void mx_pop_history_front(t_history **head) {
	t_history *temp = NULL;

	if (head && *head) {
		temp = *head;
		if (malloc_size(temp->data))
			mx_strdel(&(temp->data));
		*head = temp->next;
		free(temp);
	}
}

void mx_pop_history_back(t_history **head) {
	if (head && *head && (*head)->next) {
		t_history *temp = *head;

		while (temp->next->next)
			temp = temp->next;
		if (malloc_size(temp->data))
			mx_strdel(&(temp->data));
		free(temp->next);
		temp->next = NULL;
	}
	else if (head && *head) {
		if (malloc_size((*head)->data))
			mx_strdel(&((*head)->data));
		free(*head);
		*head = NULL;
	}
}

void mx_check_history(t_info *info, char *line) {
	if (mx_strlen(line)) {
		free(info->history_pack->history->data);
		info->history_pack->history->data = mx_strdup(line);
	}
	else
		mx_pop_history_front(&info->history_pack->history);
	if (info->history_pack->total_num < 100)
		(info->history_pack->total_num)++;
	else
		mx_pop_history_back(&info->history_pack->history);
}
