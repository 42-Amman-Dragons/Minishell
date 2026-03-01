/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by haya              #+#    #+#             */
/*   Updated: 2026/03/01 13:51:26 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int			SIGNUM = 0;


int ft_strcmp(char *s1, char *s2)
{
    int len1;
    int len2;
    
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    if(len1 != len2)
        return (-1);
    return (ft_strncmp(s1, s2, len1));
}

int is_builtin_function(char *token)
{
    if (ft_strcmp(token, "exit") == 0)
        return (1);
    return (0);
}


void parse_and_execute(char *env[], t_minishell *shell)
{

	t_list	*tokens;
	t_tree	*tree;
	int err;
    int  id;
    int status;
	
	tokens = tokenizer(shell->line);
	tree = build_ast(tokens);
	err = 0;
	init_heredocs(tree, shell);
	expander(tree, shell);
	//executor(tree, shell);
	

    
    if(!tokens)
        return ;
    if(err == 1)
    {
        free_tree(tree);
        return ;
    }
    // print_tree(node);

    id = fork();
    if (id == -1)
    {
        perror("Fork Error: ");
        err = 1;
    }
	
    if(tree->type == NODE_CMD && tree->data.cmd.args && is_builtin_function(tree->data.cmd.args[0]))
			return ;
    // execute build in functions
    else if(id == 0)
        exec_tree(tree, env);
    else{
        waitpid(id, &status, 0);
        if (WIFEXITED(status))
            err = WEXITSTATUS(status);
        printf("err value = %i\n",err);
        rl_on_new_line();
    }

	print_tree(tree);
	ft_lstclear(&tokens, free_token);
	free(shell->line);
	shell->line = NULL;
	free_tree(tree);
}



char	*get_prompt(char *username, char *servername)
{
	char	buff[PATH_MAX];
	char	*prompt;
	int		user_len;
	int		server_len;
	int		buff_len;

	ft_bzero(buff, PATH_MAX);
	if (getcwd(buff, PATH_MAX) == NULL)
		return (NULL);
	user_len = ft_strlen(username);
	server_len = ft_strlen(servername);
	buff_len = ft_strlen(buff);
	prompt = malloc(buff_len + user_len + server_len + 6);
	if (!prompt)
		return (NULL);
	ft_memcpy(prompt, username, user_len);
	ft_memcpy(prompt + user_len, "@", 1);
	ft_memcpy(prompt + user_len + 1, servername, server_len);
	ft_memcpy(prompt + user_len + server_len + 1, ":", 1);
	ft_memcpy(prompt + user_len + server_len + 2, buff, buff_len);
	ft_memcpy(prompt + user_len + server_len + buff_len, "$ \0", 3);
	return (prompt);
}

t_minishell	*init_minishell(void)
{
	t_minishell	*shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->history = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	load_history(&(shell->history));
	shell->prompt = get_prompt("haya", "dragons");
	tcgetattr(STDIN_FILENO, &(shell->original_termos));
	shell->new_termos = shell->original_termos;
	shell->new_termos.c_lflag &= ~(ECHOCTL);
	shell->new_termos.c_cc[VMIN] = 1;
	shell->new_termos.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &(shell->new_termos));
	if (!shell->prompt)
	{
		free_all(shell);
		return (NULL);
	}
	return (shell);
}

int	main(int argc,char **argv, char *env[])
{
	if(argc != 1)
		return 1;
	// dummy code to silent the warnings
	if(ft_strncmp(argv[0],"",1) == 1)
		return 1;
	t_minishell	*shell;

	shell = init_minishell();
	if (!shell)
		return (1);
	shell->env = init_mutable_env(env);
	if (!shell->env)
	{
		free_all(shell);
		return (1);
	}
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, ctrl_backslash);
	while (1)
	{
		parse_and_execute(env, shell);
		shell->line = readline(shell->prompt);
		if (SIGNUM != 0)
		{
			shell->exit_status = 128 + SIGNUM;
			SIGNUM = 0;
		}
		if (!shell->line)
		{
			printf("exit\n");
			break ;
		}
		if (add_to_history(shell->line, &(shell->history)) == -1)
		{
			free_all(shell);
			return (1);
		}
		if (ft_strncmp("exit", shell->line, 4) == 0)
		{
			// shell->exit_status = 0;
			// free(shell->line);
			// shell->line = NULL;
			break ;
		}
	}
	custom_save_history(&(shell->history));
	free_all(shell);
	return (0);
}

