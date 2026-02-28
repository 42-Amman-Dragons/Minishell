/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:22:14 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/22 22:26:56 by mabuqare         ###   ########.fr       */
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
# include <sys/wait.h>
# include <termios.h>

typedef enum e_tokenType
{
	WORD = 1,
	PIPE,
	REDIRECT,
	AND,
	OR,
	OPEN_PAREN,
	CLOSE_PAREN
}					t_tokenType;

typedef enum e_dir_mode
{
	DIR_OUT_TRUNC = 1,
	DIR_OUT_APPEND,
	DIR_IN_FILE,
	DIR_IN_HEREDOC
}					t_dir_mode;

typedef struct s_redir_data
{
	char			*filename;
	t_dir_mode		mode;
	int				heredoc_expand;
	int				heredoc_fd;
}					t_redir_data;

typedef struct s_pipe_data
{
}					t_pipe_data;

typedef struct s_and_data
{
}					t_and_data;

typedef struct s_or_data
{
}					t_or_data;

typedef struct s_word_data
{
	char			*value;
}					t_word_data;

typedef union u_token_data
{
	t_word_data		word;
	t_pipe_data		pipe;
	t_redir_data	redir;
	t_and_data		and;
	t_or_data		or;
}					t_token_data;

/*Polymorphic token*/
typedef struct s_token
{
	t_tokenType		type;
	t_token_data	data;
}					t_token;

typedef struct s_minishell
{
	char			*line;
	char			*prompt;
	char			**env;
	t_list			*history;
	struct termios	new_termos;
	struct termios	original_termos;
	int				is_interactive;
	int				exit_status;
}					t_minishell;

/*AST Node Types*/
typedef enum e_node_type
{
	NODE_CMD = 1,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}				t_node_type;

typedef struct s_cmd_data
{
	char			**args;
	t_list			*redirections;
}				t_cmd_data;

typedef struct s_oper_data
{
	struct s_tree	*left;
	struct s_tree	*right;
}				t_oper_data;

typedef struct s_subshell_data
{
	struct s_tree	*child;
	t_list			*redirections;
}				t_subshell_data;

typedef union u_node_data
{
	t_cmd_data		cmd;
	t_oper_data		oper;
	t_subshell_data	subshell;
}				t_node_data;

typedef struct s_tree
{
	t_node_type		type;
	t_node_data		data;
}				t_tree;

int				add_to_history(char *line, t_list **history);
void			load_history(t_list **history);
void			custom_save_history(t_list **history);
void			ctrl_c_handler(int signalNumber);
void			ctrl_backslash(int signalNumber);
char			**init_mutable_env(char **env);
void			free_env(char **env);
void			free_all(t_minishell *shell);
void			print_tree(t_tree *head);
void			print_arr(char **arr);

/*Parser*/
t_tree			*build_ast(t_list *tokens);
t_tree			*parse_logic_expr(t_list **cur, int *err);
t_tree			*parse_pipe_seq(t_list **cur, int *err);
t_tree			*parse_cmd_or_sub(t_list **cur, int *err);
t_tree			*parse_subshell(t_list **cur, int *err);
t_tree			*parse_simple_cmd(t_list **cur, int *err);
int				parse_redir(t_list **cur, t_list **redirs, int *err);
t_tokenType		cur_type(t_list **cur);
t_token			*advance(t_list **cur);
t_tree			*create_oper_node(t_node_type type, t_tree *l, t_tree *r);
t_tree			*create_cmd_node(char **args, t_list *redirs);
t_tree			*create_subshell_node(t_tree *child, t_list *redirs);
void			free_tree(t_tree *tree);
void			free_args(char **args);
void			free_redir(void *ptr);
void			syntax_error(t_list *cur, int *err);
const char		*token_to_str(t_token *tok);

/*Expander*/
typedef enum e_exp_state
{
	EXP_NORMAL,
	EXP_SQUOTE,
	EXP_DQUOTE
}				t_exp_state;

typedef struct s_expand
{
	char		*result;
	char		**env;
	int			i;
	int			exit_status;
	t_exp_state	state;
}				t_expand;

void			expander(t_tree *tree, t_minishell *shell);
char			*expand_word(char *word, char **env, int exit_status);
char			*expand_dollar(char *word, t_expand *ctx);
char			*append_char(char *result, char c);
char			*append_str(char *result, char *s);
void			init_heredocs(t_tree *tree, t_minishell *shell);
int				word_has_quotes(char *word);
void			strip_empty_args(t_tree *node, int count);
void			expand_one_arg(char **args, int i, t_minishell *shell);

/*Tokenizer*/
t_tokenType		identify_token(char *s);
t_list			*tokenizer(char *line);
void			free_token(void *ptr);
t_token			*creat_token(char *str, int *i);
t_token			*create_pipe_token(char *str, int *i);
t_token			*create_redirect_token(char *str, int *i);
t_token			*create_and_or_token(char *str, int *i);
t_token			*create_paren_token(char *str, int *i);
t_token			*create_word_token(char *str, int *i);
int				is_whitespace(char c);
void			skip_whitespaces(char *ptr, int *i);
t_dir_mode		identify_redirection_mode(char *str, int *i);
char			*extract_word(char *str, int *i);
int				word_boundary(char *str);

#endif