/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by haya              #+#    #+#             */
/*   Updated: 2026/03/05 13:41:04 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int	SIGNUM = 0;

static void	handle_signal_status(t_minishell *shell)
{
	if (SIGNUM != 0)
	{
		shell->exit_status = 128 + SIGNUM;
		SIGNUM = 0;
	}
}

int	runshell(t_minishell *shell)
{
	while (1)
	{
		shell->line = readline(shell->prompt);
		handle_signal_status(shell);
		if (!shell->line)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(shell->line) == 0)
		{
			free(shell->line);
			shell->line = NULL;
			continue ;
		}
		if (add_to_history(shell->line, &(shell->history)) == -1)
		{
			free_all(shell);
			shell = NULL;
			return (-1);
		}
		parse_and_execute(shell);
		free(shell->line);
		shell->line = NULL;
	}
	return (0);
}

int	main(int argc, char **argv, char *env[])
{
	t_minishell	*shell;
	(void)argv;
	if (argc != 1)
		return (1);
	shell = init_minishell();
	if (!shell)
		return (1);
	if (init_mutable_env(env, shell) != 0)
	{
		free_all(shell);
		shell = NULL;
		return (1);
	}
	set_signals_prompt();
	if (runshell(shell) == -1)
		return (1);
	custom_save_history(shell);
	free_all(shell);
	shell = NULL;
	return (0);
}
