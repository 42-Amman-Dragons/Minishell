/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:18 by haya              #+#    #+#             */
/*   Updated: 2026/03/12 11:54:29 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env(char **env)
{
	int i;

	if (!env)
		return;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i++] = NULL;
	}
	free(env);
}

void free_all(t_minishell *shell)
{
	if (!shell)
		return;
	tcsetattr(STDIN_FILENO, TCSANOW, &(shell->original_termos));
	rl_clear_history();
	if (shell->history)
		ft_lstclear(&(shell->history), free);
	shell->history = NULL;
	if (shell->prompt)
		free(shell->prompt);
	if(shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	ft_bzero(shell, sizeof(*shell));
	free(shell);
}
