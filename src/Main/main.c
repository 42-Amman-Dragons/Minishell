/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare <mabuqare@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:41:06 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int			g_signum = 0;

static int	process_line_interactive(t_minishell *shell)
{
	shell->line = readline(shell->prompt);
	handle_signal_status(shell);
	if (!shell->line)
	{
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	if (ft_strlen(shell->line) == 0)
	{
		free(shell->line);
		shell->line = NULL;
		return (0);
	}
	if (add_to_history(shell->line, &(shell->history)) == -1)
	{
		cleanup_shell(shell, 1);
		return (-1);
	}
	parse_and_execute(shell);
	free(shell->line);
	shell->line = NULL;
	return (0);
}

int	runshell(t_minishell *shell)
{
	int	status;

	while (1)
	{
		if (shell->is_interactive)
		{
			free(shell->prompt);
			shell->prompt = get_prompt(shell);
			if (!shell->prompt)
				shell->prompt = ft_strdup("minishell$ ");
		}
		if (shell->is_interactive)
			status = process_line_interactive(shell);
		else
			status = process_line_non_interactive(shell);
		if (status == 1)
			break ;
		if (status == -1)
			return (-1);
	}
	return (0);
}

int	main(int argc, char **argv, char *env[])
{
	t_minishell	*shell;

	(void)argv;
	if (argc != 1)
		return (1);
	shell = init_shell();
	if (!shell)
		return (1);
	if (init_mutable_env(env, shell) != 0)
		return (cleanup_shell(shell, 1));
	if (shell->is_interactive && init_interactive_shell(shell) != 0)
		return (cleanup_shell(shell, 1));
	if (runshell(shell) == -1)
		return (1);
	if (shell->is_interactive)
		custom_save_history(shell);
	return (cleanup_shell(shell, shell->exit_status));
}
