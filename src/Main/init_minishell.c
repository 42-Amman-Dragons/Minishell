/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:30:32 by haya              #+#    #+#             */
/*   Updated: 2026/03/13 00:21:10 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*init_minishell(void)
{
	t_minishell	*shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->history = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	if (load_history(&(shell->history)) == -1)
	{
		free(shell);
		return (NULL);
	}
	shell->prompt = NULL;
	tcgetattr(STDIN_FILENO, &(shell->original_termos));
	shell->new_termos = shell->original_termos;
	shell->new_termos.c_lflag &= ~(ECHOCTL);
	shell->new_termos.c_cc[VMIN] = 1;
	shell->new_termos.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &(shell->new_termos));
	return (shell);
}
