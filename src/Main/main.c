/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by haya              #+#    #+#             */
/*   Updated: 2026/03/17 05:55:14 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int			g_SIGNUM = 0;

static void	handle_signal_status(t_minishell *shell)
{
	if (g_SIGNUM != 0)
	{
		shell->exit_status = 128 + g_SIGNUM;
		g_SIGNUM = 0;
	}
}

void	welcome(void)
{
	printf("\033[38;5;222m ================================================================================\033[0m\n");
	printf("\033[1;32m   ____  ____      _    ____  ___  _   _ ____ \033[0;36m/\\\033[1;32m____  _   _ _____ _     _\033[0m \n");
	printf("\033[1;32m  |  _ \\|  _ \\    / \\  / ___|/ _ \\| \\ | / ___|\033[0;36m/ \\\033[1;32m / ___|| | | | ____| |   |\033[0m \n");
	printf("\033[1;32m  | | | | |_) |  / _ \\| |  _| | | |  \\| \\___ \033[0;36m/  \\\033[1;32m ___ \\| |_| |  _| | |   |\033[0m \n");
	printf("\033[1;32m  | |_| |  _ <  / ___ \\ |_| | |_| | |\\  |___)\033[0;36m\\  /\033[1;32m_) |  _  | |___| |___| |___\033[0m \n");
	printf("\033[1;32m  |____/|_| \\_\\/_/   \\_\\____|\\___/|_| \\_|____/\033[0;36m\\/\033[1;32m____/|_| |_|_____|_____|_____|\033[0m \n");
	printf("\033[0;36m                                               \\/\033[0m \n");
	printf("  Done by: Mohannad and Haya 🐉✨\n");
	printf("\033[38;5;222m ================================================================================\033[0m\n");
	printf("\n");
}

static int	process_line(t_minishell *shell)
{
	shell->line = readline(shell->prompt);
	handle_signal_status(shell);
	if (!shell->line)
	{
		printf("exit\n");
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
		free_all(shell);
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
		free(shell->prompt);
		shell->prompt = get_prompt(shell);
		status = process_line(shell);
		if (status == 1)
			break ;
		if (status == -1)
			return (-1);
	}
	return (0);
}

t_minishell	*init_shell(char **env)
{
	t_minishell	*shell;

	shell = init_minishell();
	if (!shell)
		return (NULL);
	if (init_mutable_env(env, shell) != 0)
	{
		free_all(shell);
		shell = NULL;
	}
	// here is tty ?
	set_signals_prompt();
	if (init_prompt(shell) != 0)
	{
		free_all(shell);
		shell = NULL;
	}
	welcome();
	return (shell);
}

int	main(int argc, char **argv, char *env[])
{
	t_minishell	*shell;

	(void)argv;
	if (argc != 1)
		return (1);
	shell = init_shell(env);
	if (!shell)
		return (1);
	if (runshell(shell) == -1)
		return (1);
	custom_save_history(shell);
	free_all(shell);
	shell = NULL;
	return (0);
}
