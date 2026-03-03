/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:30:32 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 13:54:29 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_prompt(char *username, char *servername)
{
	char buff[PATH_MAX];
	char *prompt;
	int user_len;
	int server_len;
	int buff_len;

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

t_minishell *init_minishell(void)
{
	t_minishell *shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->history = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	if(load_history(&(shell->history)) == -1)
		free(shell);
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
		/* no more references to shell */
		shell = NULL;
		return (NULL);
	}
	return (shell);
}
