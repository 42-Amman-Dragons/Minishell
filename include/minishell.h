#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "fcntl.h"
#include <sys/stat.h>

int add_to_history(char *line, t_list **history);
void load_history(t_list **history);
void custom_save_history(t_list **history);
void ctrl_c_handler(int signalNumber);
void ctrl_backslash(int signalNumber) ;
#endif