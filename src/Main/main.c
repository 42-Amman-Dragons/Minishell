/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by haya              #+#    #+#             */
/*   Updated: 2026/03/12 17:29:25 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int g_SIGNUM = 0;

static void handle_signal_status(t_minishell *shell)
{
	if (g_SIGNUM != 0)
	{
		shell->exit_status = 128 + g_SIGNUM;
		g_SIGNUM = 0;
	}
}

void welcome()
{

	// =================================================================================
	//   ____  ____      _    ____  ___  _   _ ____   /\____  _   _ _____ _     _
	//  |  _ \|  _ \    / \  / ___|/ _ \| \ | / ___| / / ___|| | | | ____| |   | |
	//  | | | | |_) |  / _ \| |  _| | | |  \| \___ \/  \___ \| |_| |  _| | |   | |
	//  | |_| |  _ <  / ___ \ |_| | |_| | |\  |___) |   _/_) |  _  | |___| |___| |___
	//  |____/|_| \_\/_/   \_\____|\___/|_| \_|____/ \ |____/|_| |_|_____|_____|_____|
	// 											      \/
	// =================================================================================

	// \033[1;32m ____  ____      _    ____  ___  _   _ ____ \033[0;36m  /\\033[1;32m____  _   _ _____ _     _		\033[0m \n
	// \033[1;32m|  _ \|  _ \    / \  / ___|/ _ \| \ | / ___|\033[0;36m / \033[1;32m/ ___|| | | | ____| |   | |		\033[0m \n
	// \033[1;32m| | | | |_) |  / _ \| |  _| | | |  \| \___ \\033[0;36m/  \033[1;32m\___ \| |_| |  _| | |   | |		\033[0m \n
	// \033[1;32m| |_| |  _ <  / ___ \ |_| | |_| | |\  |___) |\033[0;36m  /\033[1;32m___) |  _  | |___| |___| |___	\033[0m \n
	// \033[1;32m|____/|_| \_\/_/   \_\____|\___/|_| \_|____/\033[0;36m \ \033[1;32m|____/|_| |_|_____|_____|_____| \033[0m \n
	//

	printf("\033[38;5;222m ================================================================================\033[0m\n");
	printf("\033[1;32m   ____  ____      _    ____  ___  _   _ ____ \033[0;36m /\\\033[1;32m____  _   _ _____ _     _		\033[0m \n");
	printf("\033[1;32m  |  _ \\|  _ \\    / \\  / ___|/ _ \\| \\ | / ___|\033[0;36m/ \033[1;32m/ ___|| | | | ____| |   | |		\033[0m \n");
	printf("\033[1;32m  | | | | |_) |  / _ \\| |  _| | | |  \\| \\___ \033[0;36m/  \033[1;32m\\___ \\| |_| |  _| | |   | |		\033[0m \n");
	printf("\033[1;32m  | |_| |  _ <  / ___ \\ |_| | |_| | |\\  |___)\033[0;36m\\   \033[1;32m_\033[0;36m/\033[1;32m_) |  _  | |___| |___| |___	\033[0m \n");
	printf("\033[1;32m  |____/|_| \\_\\/_/   \\_\\____|\\___/|_| \\_|____/\033[0;36m\\ \033[1;32m|____/|_| |_|_____|_____|_____| \033[0m \n");
	printf("                                               \033[0;36m\\/\033[0m \n");
	printf("  Done by: Mohannad and Haya 🐉✨\n");
	printf("\033[38;5;222m ================================================================================\033[0m\n");
	printf("\n");
}

int runshell(t_minishell *shell)
{
	while (1)
	{
		shell->prompt = get_prompt(shell);
		shell->line = readline(shell->prompt);
		handle_signal_status(shell);
		if (!shell->line)
		{
			printf("exit\n");
			break;
		}
		if (ft_strlen(shell->line) == 0)
		{
			free(shell->line);
			shell->line = NULL;
			continue;
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

int init_prompt(t_minishell *shell)
{
	shell->prompt = get_prompt(shell);
	if (!shell->prompt)
		return (-1);
	return (0);
}

int main(int argc, char **argv, char *env[])
{
	t_minishell *shell;
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
	if (init_prompt(shell) != 0)
	{
		free_all(shell);
		shell = NULL;
		return (1);
	}
	welcome();
	if (runshell(shell) == -1)
		return (1);
	custom_save_history(shell);
	free_all(shell);
	shell = NULL;
	return (0);
}
