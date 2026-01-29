#ifndef MINISHELL_H
#define MINISHELL_H

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "fcntl.h"
#include <termios.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/ioctl.h>


typedef struct s_minishell
{
    char *line;
    char *prompt;
    t_list *history;
    struct termios new_termos;
    struct termios original_termos;
    int is_interactive;
    int exit_status;
} t_minishell;

int add_to_history(char *line, t_list **history);
void load_history(t_list **history);
void custom_save_history(t_list **history);
void ctrl_c_handler(int signalNumber);
void ctrl_backslash(int signalNumber);
void free_all(t_minishell *shell);

#endif