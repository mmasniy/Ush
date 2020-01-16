#include "ush.h"

char **mx_ush_split_line(char *line) {
	int bufsize = USH_TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;
	
	if (!tokens) {
		fprintf(stderr, "Ush: allocation error\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, USH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += USH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "Ush: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, USH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}
