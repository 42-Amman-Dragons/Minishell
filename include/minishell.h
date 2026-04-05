/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:22:14 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/05 09:28:40 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # define _DEFAULT_SOURCE
# include "fcntl.h"
# include "libft.h"
# include <dirent.h>
# include <errno.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

# define DRAGON_GREEN "\001\033[1;32m\002"
# define DRAGON_CYAN "\001\033[0;36m\002"
# define PROMPT_EMOJI "🐉"
# define RESET "\001\033[0m\002"

typedef enum e_tokenType
{
	WORD = 1,
	PIPE,
	REDIRECT,
	AND,
	OR,
	OPEN_PAREN,
	CLOSE_PAREN,
	ASTERISK
}					t_tokenType;

typedef enum e_dir_mode
{
	DIR_OUT_TRUNC = 1,
	DIR_OUT_APPEND,
	DIR_IN_FILE,
	DIR_IN_HEREDOC,
	NOT_FOUND
}					t_dir_mode;

typedef struct s_redir_data
{
	char			*filename;
	t_dir_mode		mode;
	int				heredoc_expand;
	int				heredoc_fd;
}					t_redir_data;

typedef struct s_word_data
{
	char			*value;
}					t_word_data;

typedef union u_token_data
{
	t_word_data		word;
	t_redir_data	redir;
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
	t_list			*openfiles;
	struct termios	new_termos;
	struct termios	original_termos;
	int				is_interactive;
	int				is_child;
	int				exit_status;
	char			*username;
	char			*servername;
	int				builtin_temp_stdin;
	int				builtin_temp_stdout;
	struct s_tree	*current_tree;
}					t_minishell;

/*AST Node Types*/
typedef enum e_node_type
{
	NODE_CMD = 1,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}					t_node_type;

typedef struct s_cmd_data
{
	char			**args;
	t_list			*redirections;
}					t_cmd_data;

typedef struct s_oper_data
{
	struct s_tree	*left;
	struct s_tree	*right;
}					t_oper_data;

typedef struct s_subshell_data
{
	struct s_tree	*child;
	t_list			*redirections;
}					t_subshell_data;

typedef union u_node_data
{
	t_cmd_data		cmd;
	t_oper_data		oper;
	t_subshell_data	subshell;
}					t_node_data;

typedef struct s_tree
{
	t_node_type		type;
	t_node_data		data;
}					t_tree;

int					add_to_history(char *line, t_list **history);
int					load_history(t_minishell *shell);
void				custom_save_history(t_minishell *shell);
void				handle_sigint(int sig);
void				handle_signal_status(t_minishell *shell);
void				set_signals_prompt(void);
void				set_signals_exec(void);
void				set_signals_child(void);
void				set_signals_heredoc(void);
int					init_mutable_env(char **env, t_minishell *shell);
void				free_env(char **env);
char				*get_env_value(char *name, char **env);
int					set_env_value(char *name, char *value, t_minishell *shell);
void				del_env_value(char *name, t_minishell *shell);
int					add_env(char *entry, t_minishell *shell);
char				*mk_env_entry(char *name, char *value);
int					cleanup_shell(t_minishell *shell, int exit_code);
int					calc_len_args(char **args);
void				update_prompt_path(t_minishell *shell);

/*Parser*/
t_tree				*build_ast(t_list *tokens, int *err);
t_tree				*parse_logic_expr(t_list **cur, int *err);
t_tree				*parse_pipe_seq(t_list **cur, int *err);
t_tree				*parse_cmd_or_sub(t_list **cur, int *err);
t_tree				*parse_subshell(t_list **cur, int *err);
t_tree				*parse_simple_cmd(t_list **cur, int *err);
int					parse_redir(t_list **cur, t_list **redirs, int *err);
t_tokenType			cur_type(t_list **cur);
t_token				*advance(t_list **cur);
t_tree				*create_oper_node(t_node_type type, t_tree *l, t_tree *r,
						int *err);
t_tree				*create_cmd_node(char **args, t_list *redirs, int *err);
t_tree				*create_subshell_node(t_tree *child, t_list *redirs,
						int *err);
void				free_tree(t_tree *tree);
void				free_args(char **args);
void				free_redir(void *ptr);
void				syntax_error(t_list *cur, int *err);
const char			*token_to_str(t_token *tok);

/*Expander*/
typedef enum e_exp_state
{
	EXP_NORMAL,
	EXP_SQUOTE,
	EXP_DQUOTE
}					t_exp_state;

typedef struct s_expand
{
	char			*result;
	char			**env;
	int				i;
	int				exit_status;
	t_exp_state		state;
}					t_expand;

void				expander(t_tree *tree, t_minishell *shell);
char				*expand_word(char *word, char **env, int exit_status);
char				*expand_word_heredoc(char *word, char **env,
						int exit_status);
char				**expand_one_arg(char **args, int i, t_minishell *shell);
void				strip_empty_args(t_tree *node, int count);
int					calc_len_args(char **args);
void				free_args(char **args);
char				*expand_dollar(char *word, t_expand *ctx);
char				*append_char(char *result, char c);
char				*append_str(char *result, char *s);
int					init_heredocs(t_tree *tree, t_minishell *shell);
int					setup_heredoc_fd(t_redir_data *rd, t_minishell *shell,
						int idx);
int					read_heredoc_nointeractive(t_redir_data *rd,
						t_minishell *shell, char *tmp);
void				push_heredoc_line(int fd, char *line, t_redir_data *rd,
						t_minishell *shell);
void				print_eof_warning(char *limiter);
int					word_has_quotes(char *word);
void				strip_empty_args(t_tree *node, int count);
char				**add_to_args(char **args, int i, char *expanded,
						int *flags);
char				*get_unquoted_var_val(char *word, int *i, char **env,
						int exit_status);
int					unquoted_dollar_has_space(char *word, char **env,
						int exit_status);
int					calc_arr_len(char **arr);
/*Tokenizer*/
t_tokenType			identify_token(char *s);
t_list				*tokenizer(char *line, int *err);
void				free_token(void *ptr);
t_token				*create_token(char *str, int *i, int *err);
t_token				*create_pipe_token(int *i);
t_token				*create_redirect_token(char *str, int *i);
t_token				*create_and_or_token(char *str, int *i);
t_token				*create_paren_token(char *str, int *i);
t_token				*create_word_token(char *str, int *i, int *err);
int					is_whitespace(char c);
void				skip_whitespaces(char *ptr, int *i);
t_dir_mode			identify_redirection_mode(char *str, int *i);
char				*extract_word(char *str, int *i, int *err);
int					word_boundary(char *str, int *err);

/*Builtins*/
int					is_builtin(char *cmd);
int					ft_echo(char **args);
int					ft_cd(char **args, t_minishell *shell);
int					ft_pwd(t_minishell *shell);
int					ft_export(char **args, t_minishell *shell);
int					print_sorted_env(t_minishell *shell);
int					ft_unset(char **args, t_minishell *shell);
int					ft_env(t_minishell *shell);
int					ft_exit(char **args, t_minishell *shell);
int					call_builtin(int idx, char **args, t_minishell *shell);
int					check_boundries(const char *nptr);

// Execution
int					exec_tree(t_tree *node, t_minishell *shell);
int					exec_cmd(t_tree *node, t_minishell *shell);
int					exec_pipe(t_tree *node, t_minishell *shell);
int					exec_and_or(t_tree *node, t_minishell *shell);
int					exec_subshell(t_tree *node, t_minishell *shell);
void				free_and_exit(t_tree *node, t_minishell *shell,
						int exit_code);
void				handle_cmd_error(char *cmd_name, t_tree *node,
						t_minishell *shell);
int					path_is_unset(t_minishell *shell);
void				update_underscore_var(t_tree *node, t_minishell *shell);
int					redir_has_ambiguous_target(t_redir_data *rd);
int					print_ambiguous_redirect(void);
int					pipe_error_and_close(int *temp_std);

// Pipe utils
int					wait_all(pid_t pid, t_minishell *shell);
int					temp_redir(int *temp_stdin, int *temp_stdout);
void				safe_close(int *fd, char *msg);
int					restore_redir(int *temp_stdin, int *temp_stdout);
int					restore_close_redir(int *fd, int *temp_stdin,
						int *temp_stdout);
// Execution utils
int					child_exit_status(int status);
void				print_sigquit_if_needed(int status, t_minishell *shell);
void				secure_close(int fd, t_tree *node, t_minishell *shell);
void				free_splitted(char **splitted);
// Redirections
int					handle_redirections(t_tree *node);
int					redirect_input(t_redir_data *rd);
int					redirect_output(t_redir_data *rd);
int					redirect_append(t_redir_data *rd);
int					redirect_heredoc(t_redir_data *rd);

// Main
t_minishell			*init_shell(void);
int					init_terminal(t_minishell *shell);
int					init_interactive_shell(t_minishell *shell);
void				parse_and_execute(t_minishell *shell);
char				*get_prompt(t_minishell *shell);
int					is_command_a_directory(const char *path);
char				*absoulute_path(char *cmd, char **env);
char				*safe_join(char *str1, char *str2);
void				normalize_spaces(char *s);
void				print_welcome_message(void);

void				consider_home_dir(char *buff, char **env);
void				change_color(char **prompt, char *color);
void				prepare_prompt_beggining(char **prompt, t_minishell *shell);
void				prepare_prompt_path(char **prompt, char *buff);
void				init_prompt(t_minishell *shell);
int					process_line_non_interactive(t_minishell *shell);

void				close_tracked_fds(t_minishell *shell);
void				track_fd(t_minishell *shell, int *heredoc_fd_ptr);
void				syntax_err_msg(char *token);
#endif