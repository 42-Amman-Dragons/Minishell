/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:18 by haya              #+#    #+#             */
/*   Updated: 2026/04/05 16:53:11 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i++] = NULL;
	}
	free(env);
}

int	cleanup_shell(t_minishell *shell, int exit_code)
{
	if (!shell)
		return (exit_code);
	tcsetattr(STDIN_FILENO, TCSANOW, &(shell->original_termos));
	rl_clear_history();
	if (shell->history)
		ft_lstclear(&(shell->history), free);
	shell->history = NULL;
	if (shell->prompt)
		free(shell->prompt);
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	close_tracked_fds(shell);
	ft_bzero(shell, sizeof(*shell));
	free(shell);
	return (exit_code);
}
