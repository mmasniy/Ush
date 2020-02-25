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

void mx_del_slash_and_quotes_in_list(t_tok **tok) {
    for (t_tok *tmp = *tok; tmp; tmp = tmp->next) {
        if (tmp->type == 0 && tmp->token) {
            if (tmp->token[0] == '\"' || tmp->token[0] == '\'') {
                mx_del_and_set(&(tmp->token)
                    , strndup(&(tmp->token[1]), strlen(tmp->token) - 2));
            }
            else {
                search_just_slash(&(tmp->token));
            }
        }
    }//////////////////////////////////////////////////////////////////
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
}

// static int find_quotes(char *line, char *type) {
//     int quotes_1 = mx_char_block(line, '\\', '\'', '\0');
//     int quotes_2 = mx_char_block(line, '\\', '"', '\0');

//     if (type == '\'')
//         return quotes_1;
//     else if (type == '"')
//         return quotes_2;
//     else {
//         if (quotes_1 == -1 || (quotes_1 > quotes_2 && quotes_2 != -1)) {
//             *type = '"';
//             return quotes_2;
//         }
//         *type = '\'';
//         return quotes_1;
//     }
// }

// void mx_del_slash_and_quotes_in_list(t_tok **tok) {
//     for (t_tok *tmp = *tok; tmp; tmp = tmp->next) {
//         if (tmp->type == 0 && tmp->token) {
//             int pos = 0;
//             int start = -1;
//             char type = 0;

//             for (int i = 0; (i = find_quotes(&(tmp->token) + pos, &type)) >= 0, type = 0) {
                
//             }

//         }
//     }

// }
/*
шукаємо ' або " кавички в залежності від того, що йде першим
після того і цих кавичках починаємо затирати \ при умові, що це " кавички

індекс кінцевої кавички + 1, стає індексом пошуку і таким чином ми знатимемо
, що необхідно перевірити перед новими кавичками*/
