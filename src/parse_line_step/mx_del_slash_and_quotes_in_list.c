#include "../../inc/ush.h"

static void search_just_slash(char **line) {
    char *new_line = mx_strnew(mx_strlen(*line));
    int pos = 0;
    int i = 0;

    for (; (pos = mx_get_char_index(&((*line)[i]), '\\')) >= 0; i++) {
        if (pos >= 0) {
            strncat(new_line, &((*line)[i]), pos);
            i += pos;
            if ((*line)[i + 1] == '\\' && strcat(new_line, "\\"))
                i++;
        }
        else {
            strcat(new_line, &((*line)[i]));
            break;
        }
    }
    strcat(new_line, &((*line)[i]));
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}

// void mx_del_slash_and_quotes_in_list(t_tok **tok) {
//     for (t_tok *tmp = *tok; tmp; tmp = tmp->next) {
//         if (tmp->type == 0 && tmp->token) {
//             if (tmp->token[0] == '\"' || tmp->token[0] == '\'') {
//                 mx_del_and_set(&(tmp->token)
//                     , strndup(&(tmp->token[1]), strlen(tmp->token) - 2));
//             }
//             else {
//                 search_just_slash(&(tmp->token));
//             }
//         }
//     }
    //////////////////////////////////////////////////////////////////
    // char *tmp_line;

    // for (t_tok *tmp = *tok; tmp; tmp = tmp->next) {
    //     if (tmp->type == 0 && tmp->token) {
    //         if (mx_get_substr_index_without_symbol(tmp->token) >= 0) {
    //             search_just_slash(&(tmp->token));
    //         }
    //         else
    //             search_just_slash(&(tmp->token));
    //     }
    // }////////////////////////////////////////////////////////////
// }

static int find_quote(char *line, char *type) {
    int quotes_1 = mx_char_block(line, '\\', '\'', '\0');
    int quotes_2 = mx_char_block(line, '\\', '"', '\0');

    if (*type == '\'')
        return quotes_1;
    else if (*type == '"')
        return quotes_2;
    else {
        if (quotes_1 == -1 || (quotes_1 > quotes_2 && quotes_2 != -1)) {
            *type = '"';
            return quotes_2;
        }
        *type = '\'';
        return quotes_1;
    }
}

static void del_slash_for_double_quotes(char **line) {
    char *new_line = mx_strnew(mx_strlen(*line));
    int pos = 0;
    int i = 0;

    for (; (pos = mx_get_char_index(&((*line)[i]), '\\')) >= 0; i++) {
        strncat(new_line, &((*line)[i]), pos);
        i += pos;
        if ((*line)[i + 1] == '\\' && strcat(new_line, "\\"))
            i++;
        else
            strcat(new_line, "\\");
    }
    strcat(new_line, &((*line)[i]));
    free(*line);
    *line = mx_strdup(new_line);
    mx_strdel(&new_line);
}

static bool work_with_line(char **tmp_line, char **new, int *pos, char *line) {
    char type = 0;

    for (int i = 0; (i = find_quote(line + *pos, &type)) >= 0; *pos += i + 1) {
        *tmp_line = strndup(line + *pos, i);
        search_just_slash(tmp_line);
        strcat(*new, *tmp_line);
        *pos += i + 1;
        if ((i = find_quote(line + *pos, &type)) == -1) {
            printf("Quotes (%c) must have a pair !\n", type);
            return 1;
        }
        else if (type == '"') {
            mx_del_and_set(tmp_line, strndup(&(line[*pos]), i));
            del_slash_for_double_quotes(tmp_line);
            strcat(*new, *tmp_line);
        }
        type == '\'' ? strncat(*new, line + *pos, i) : 0;
        mx_strdel(tmp_line);
    }
    return 0;
}

bool mx_del_slash_and_quotes_in_list(t_tok **tok, bool *not_valid) {
    int pos = 0;
    char *new_line = NULL;
    char *tmp_line = NULL;

    for (t_tok *tmp = *tok; tmp; tmp = tmp->next, pos = 0) {
        if (tmp->type == 0 && tmp->token) {
            new_line = mx_strnew(strlen(tmp->token));
            if (work_with_line(&tmp_line, &new_line, &pos, tmp->token)) {
                mx_strdel(&new_line);
                mx_strdel(&tmp_line);
                return (*not_valid = 1);
            }
            tmp_line = strdup(tmp->token + pos);
            search_just_slash(&tmp_line);
            strcat(new_line, tmp_line);
            mx_del_and_set(&(tmp->token), strdup(new_line));
            mx_strdel(&new_line);
            mx_strdel(&tmp_line);
        }
    }
    return 0;
}

// bool mx_del_slash_and_quotes_in_list(t_tok **tok, bool *not_valid) {
//     int pos = 0;
//     char type = 0;
//     char *new_line = mx_strnew(strlen(tmp->token));
//     char *tmp_line = NULL;

//     for (t_tok *tmp = *tok; tmp; tmp = tmp->next) {
//         if (tmp->type == 0 && tmp->token) {
//             pos = type = 0;
//             new_line = mx_strnew(strlen(tmp->token));
//             for (int i = 0; (i = find_quote(tmp->token + pos, &type)) >= 0; ) {
//                 tmp_line = strndup(tmp->token + pos, i);
//                 search_just_slash(&tmp_line);
//                 strcat(new_line, tmp_line);
//                 pos += i + 1;
//                 if ((i = find_quote(tmp->token + pos, &type)) == -1) {
//                     *not_valid = 1;
//                     printf("Quotes (%c) must have a pair !\n", type);
//                     return;
//                 }
//                 else if (type == '\'')
//                     strncat(new_line, tmp->token + pos, i);
//                 else if (type == '"') {
//                     mx_del_and_set(&tmp_line, strndup(tmp->token + pos, i));
//                     del_slash_for_double_quotes(&tmp_line);
//                     strcat(new_line, tmp_line);
//                 }
//                 pos += i + 1;
//                 mx_strdel(&tmp_line);
//                 type = 0;
//             }
//             tmp_line = strdup(tmp->token + pos);
//             search_just_slash(&tmp_line);
//             strcat(new_line, tmp_line);
//             mx_strdel(&(tmp->token));
//             tmp->token = strdup(new_line);
//             mx_strdel(&new_line);
//         }
//     }
// }

/*
шукаємо ' або " кавички в залежності від того, що йде першим
після того і цих кавичках починаємо затирати \ при умові, що це " кавички

індекс кінцевої кавички + 1, стає індексом пошуку і таким чином ми знатимемо
, що необхідно перевірити перед новими кавичками*/
