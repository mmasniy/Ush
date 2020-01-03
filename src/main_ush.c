#include "ush.h"

int main(int argc, char **argv) {
    // Загрузка файлов конфигурации при их наличии.
    t_info  *info_sh = malloc(sizeof(t_info));

    info_sh->argc = argc;
    info_sh->argv = argv;
    // info_sh->env = env;
    // Запуск цикла команд.
    lsh_loop(info_sh);
    // Выключение / очистка памяти.
    return EXIT_SUCCESS;
}

void lsh_loop(t_info *info_sh) {
    char *line;
    char **args;
    int status = 1;
    
    if (info_sh->argc) {}
    while (status) {
    printf("%s%s%s", GRN, "u$h> ", RESET);
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);
    free(line);
    free(args);
    }
}

