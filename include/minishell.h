/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:22:14 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/16 13:47:47 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # define _DEFAULT_SOURCE
# include "fcntl.h"
# include "libft.h"
# include <errno.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
#include <sys/wait.h>
# include <termios.h>

typedef struct s_minishell
{
	char			*line;
	char			*prompt;
	t_list			*history;
	struct termios	new_termos;
	struct termios	original_termos;
	int				is_interactive;
	int				exit_status;
}					t_minishell;

typedef struct s_token
{
	char			*type;
	char			*content;
}					t_token;

typedef struct s_tree
{
	char *type;
	char **argumnts;
	char *redir_type;
	char *filename;

	struct s_tree *left;
	struct s_tree *right;
} t_tree;

// enum TokenTypes {
//     WORD,
//     MONDAY,
//     TUESDAY,
//     WEDNESDAY,
//     THURSDAY,
//     FRIDAY,
//     SATURDAY
// };


int					add_to_history(char *line, t_list **history);
void				load_history(t_list **history);
void				custom_save_history(t_list **history);
void				ctrl_c_handler(int signalNumber);
void				ctrl_backslash(int signalNumber);
void				free_all(t_minishell *shell);
void				*tokeniztion(char *line);
t_tree				*parse_and_execute(t_list *tokens, char *env[]);
void				exec_tree(t_tree *node, char *env[]);
void				print_tree(t_tree *head);
t_tree				*create_comand_node(t_list **tokens);
void				print_arr(char **arr);
t_tree				*parse_tree(t_list *tokens, int *parse_error);
int					is_builtin_function(char *token);
void				execute(t_tree *node, char *env[]);
int ft_strcmp(char *s1, char *s2);
#endif