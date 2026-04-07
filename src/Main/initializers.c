/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:30:32 by hal-law           #+#    #+#             */
/*   Updated: 2026/04/07 17:40:14 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_prompt(t_minishell *shell)
{
	print_welcome_message();
	shell->prompt = get_prompt(shell);
}

t_minishell	*init_shell(void)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->openfiles = NULL;
	shell->builtin_temp_stdin = -1;
	shell->builtin_temp_stdout = -1;
	return (shell);
}

int	init_interactive_shell(t_minishell *shell)
{
	set_signals_prompt();
	if (load_history(shell) != 0)
		return (-1);
	init_prompt(shell);
	return (0);
}
