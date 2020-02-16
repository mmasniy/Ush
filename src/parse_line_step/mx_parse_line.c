#include "../../inc/ush.h"

// static void replace_quotes_spaces(char **line, char c) {
//     int pos = 0;
//     char *chk = NULL;
//     char *replace = NULL;

//     for (int i = 0; (i = mx_char_block(&((*line)[pos])
//         , '\\', c, '\0')) >= 0; ) {
//         pos += i;
//         if ((i = mx_char_block(&((*line)[pos + 1]), '\\', c, '\0')) >= 0) {
//             chk = mx_strndup(&((*line)[pos + 1]), i);
//             pos += i + 1;
//             printf("chk = |%s|\n", chk);
//             replace = mx_replace_substr(chk, " ", "Zu5Qmpnr");
//             mx_del_and_set(&chk, mx_replace_substr(chk, " ", replace));
//         }
//         else
//             break;
//     }
// }

// void mx_replace_spaces_and_slash(char **line) {
//     replace_quotes_spaces(line, '\"');
//     replace_quotes_spaces(line, '\'');
//     // mx_del_and_set(line, mx_replace_substr(*line, "\\ ", "Zu5Qmpnr"));
// }

void mx_parse_line(t_info *info, char **line) {
    mx_tilde_work(info, line, *line);
    // printf("line = %s\n", *line);
    mx_execute_substitutions(info, line);
    // mx_replace_spaces_and_slash(line);
    if (info->status == 1) {
        printf("Error !\n");
        return;
    }
    // printf("-------------------------------------\n%s\n-------------------------------------\n", *line);
}


// echo Hello ` \"    \`   ls \" \\\` ls \\\` \"  \` \"  `

// echo Hello ` \"    \`   ls \" \\\` ls \\\` \"  \` \"  `
// echo Hello  \"       ls \"  ls  \"   \"  

// 1 = шукаємо `
// 2 = визначаємо чи є він в якихось кавичках (кожеш раз, як спускаємося на рівень нище)
// 3 = відправляємо ці ` в рекурсію дальше, але перед тим затираємо // чи /`
// 4 = заглиблюємося максимально вниз по рекурсії і з кінця починаємо виконувати парсинг
// Етапи парсингу:
// ...


// static void search_just_slash(char **line) {
//     char *new_line = mx_strnew(mx_strlen(*line));
//     int pos = 0;
//     int i = 0;

//     for (; (pos = mx_get_char_index(&((*line)[i]), '\\')) >= 0; i++) {
//         if (pos >= 0) {
//             strncat(new_line, &((*line)[i]), pos);
//             i += pos;
//             if ((*line)[i + 1] == '\\' && strcat(new_line, "\\"))
//                 i++;
//         }
//         else {
//             strcat(new_line, &((*line)[i]));
//             break;
//         }
//     }
//     strcat(new_line, &((*line)[i]));
//     free(*line);
//     *line = mx_strdup(new_line);
//     mx_strdel(&new_line);
// }

// #include "../../inc/ush.h"

// static char findchar(char c);
// static void search_just_slash(char **line);

// void mx_parse_line(t_info *info, char **line) {
//     search_just_slash(line);
//     if (mx_get_char_index(*line, '$') >= 0)
//         mx_insert_value(info, line, *line);
//     mx_tilde_search(line, *line);// ~
//     if (mx_get_char_index(*line, '=') >= 0)
//         mx_find_and_add_key_value(info, line, *line);
//     mx_shell_functions(info, line);
// }

bool mx_is_allowed_symbol_for_tilde(char c) {
    if ((c > 47 && c < 58) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)
        || c == '-' || c == '_')
        return 1;
    return 0;
}

bool mx_tilde_search(t_info *info, char **line) {
    for (int pos = 0; (*line)[pos]; pos++) {
        if (mx_skip_single_quotes(*line, &pos)
            || mx_skip_double_quotes(*line, &pos)
            || mx_skip_substitutions(*line, &pos)) {
            return (info->status = 1);
        }
        printf("+%s+\n", &((*line)[pos]));
        if ((*line)[pos] == '~') {
        }
    }
    return 0;
}

static char findchar(char c) {
    char t = -1;

    t = c == 'n' ? 10 :
    c == 't' ? 9 :
    c == 'v' ? 11 :
    c == 'b' ? 8 :
    c == 'a' ? 7 :
    c == 'r' ? 013 :
    c == '\"' ? 34 :
    c == '\'' ? 39 :
    c == '\?' ? 63 :
    c == '\\' ? 92 :
    c == '\0' ? 0 : -1;
    return t;
}

void mx_search_slash(char **line) { 
    char type = -1;
    char *craft = *line;
    char *new_line = mx_strnew(mx_strlen(*line));

    for (int i = 0; (i = mx_get_char_index(craft, '\\')) >= 0;) {
        if (i >= 0 && craft[i + 1] && ((type = findchar(craft[i + 1])) >= 0)) {
            strncat(new_line, craft, i);
            new_line[mx_strlen(new_line)] = type;
        }
        else
            strncat(new_line, craft, i + 2);
        if (craft[i + 1])
            craft += i + 2;
        else
            craft += i + 1;
    }
    strcat(new_line, craft);
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}
