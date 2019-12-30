#include "ush.h"
#define LSH_RL_BUFSIZE 1024

char *lsh_read_line(void);
void lsh_loop(void);

int main(int argc, char **argv){
	// Загрузка файлов конфигурации при их наличии.

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
    fprintf(stderr, "lsh: ошибка выделения памяти\n");
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
        fprintf(stderr, "lsh: ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void lsh_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    printf("u$h> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}
