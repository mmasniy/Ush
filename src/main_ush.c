#include "ush.h"

char *lsh_read_line(void);
void lsh_loop(void);
char **lsh_split_line(char *line);

int main(int argc, char **argv){
	// Загрузка файлов конфигурации при их наличии.
  (void)argc;
  (void)argv;
	// Запуск цикла команд.
	lsh_loop();

	// Выключение / очистка памяти.

  return EXIT_SUCCESS;
}

char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "ush: ошибка выделения памяти\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Читаем символ
    c = getchar();

    // При встрече с EOF заменяем его нуль-терминатором и возвращаем буфер
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // Если мы превысили буфер, перераспределяем блок памяти
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "ush: ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: ошибка выделения памяти\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

void lsh_loop(void) {
  char *line;
  char **args;
  int status = 1;

  while (status) {
    printf("%s%s%s", GRN, "u$h> ", RESET);
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  }
}

