#include "ush.h"

t_info *mx_info_start(t_info *info, char **environ) {
  
  int num = 0;
  for (int i = 0; info->builtin_str[i]; i++)
    num++;
  info->env_o = environ;
  info->env_c = environ;
  info->num_of_func = num;
  info->args = NULL;
  return info;
}
