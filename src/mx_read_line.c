#include "ush.h"

// static int str_filter(t_info *info, char **str, int *position, char *c);
// static void home_end_page(t_info *info, char **buf, int *position, char *c);

// char *mx_ush_read_line(t_info *info) {
//     int bufsize = USH_RL_BUFSIZE;
//     int position = 0;
//     char *buffer = mx_strnew(bufsize);
//     char c[4];

//     if (!buffer) {
//         fprintf(stderr, "Ush: allocation error\n");
//         exit(EXIT_FAILURE);
//     }
//     mx_push_history_front(&info->history_pack->history, buffer);
//     info->history_pack->pos = info->history_pack->history;
//     mx_print_ush(info);
//     while (1) {
//         read(STDIN_FILENO, c, 4);
//         // if (c[0] == )
//         printf("%d %d %d %d\n", c[0], c[1], c[2], c[3]);
//         if (c[0] == ENTER || c[0] == '\n') {
//             write(1, "\n\r", 2);
//             return buffer;
//         }
//         else {
//             if (str_filter(info, &buffer, &position, c) == CTRL_C) {
//                 write(1, "\n\r", 2);
//                 info->ctrl_c = 1;
//                 return buffer;
//             }
//             mx_print_line(info, buffer, position);
//         }
//         if ((size_t)position + 1 >= malloc_size(buffer)
//             || malloc_size(buffer) <= (size_t)mx_strlen(buffer) + 1) {
//             bufsize += USH_RL_BUFSIZE;
//             buffer = realloc(buffer, bufsize);
//             if (!buffer) {
//                 fprintf(stderr, "Ush: allocation error\n");
//                 exit(EXIT_FAILURE);
//             }
//         }
//     }
// }



// static void home_end_page(t_info *info, char **buf, int *position, char *c) {
//     if (c[2] == 72)
//         *position = 0;
//     else if (c[2] == 70)
//         *position = mx_strlen(*buf);
//     else if (c[2] == 53 && info->history_pack->pos)
//         while (info->history_pack->pos->next)
//             mx_arrows_exec(info, buf, position, 65);
//     else if (c[2] == 54 && info->history_pack->pos)
//         while (info->history_pack->pos->prev)
//             mx_arrows_exec(info, buf, position, 66);
// }

// static int str_filter(t_info *info, char **buffer, int *position, char *c) {
//     if (c[0] == 27) {
//         if (c[2] >= 65 && c[2] <= 68)
//             mx_arrows_exec(info, buffer, position, c[2]);
//         else if (c[2] == 72 || c[2] == 70 || c[2] == 53 || c[2] == 54)
//             home_end_page(info, buffer, position, c);
//     }
//     else if (c[0] >= 32 && c[0] <= 127) {
//         mx_str_edit(info, *buffer, position, c);
//     }
//     else {
//         mx_line_hot_key(info, buffer, position, c);
//         return c[0];
//     }
//     return c[0] >= 32 && c[0] <= 127 ? 0 : c[0];
// }

// else if (c[0] <= -46 && c[0] >= -48) { // Кирилиця
//     int len = mx_strlen(*buffer);

//     if (buffer[0])
//         for (int i = len; i > *position; i--)
//             (*buffer)[i] = (*buffer)[i - 1];
//     (*buffer)[*position] = c[0];
//     (*buffer)[*position + 1] = c[1];
//     (*buffer)[len + 2] = '\0';
//     (*position)+= 2;
// }
