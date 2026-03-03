/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:23 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 13:46:27 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int SIGNUM = 0;


int runshell(t_minishell *shell)
{
	while (1)
	{
		parse_and_execute(shell);
		shell->line = readline(shell->prompt);
		if (SIGNUM != 0)
		{
			shell->exit_status = 128 + SIGNUM;
			SIGNUM = 0;
		}
		if (!shell->line)
		{
			printf("exit\n");
			break;
		}
		if (add_to_history(shell->line, &(shell->history)) == -1)
		{
			free_all(shell);
			shell = NULL;
			return (-1);
		}
		if (ft_strncmp("exit", shell->line, 4) == 0)
		{
			break;
		}
	}
	return(0);
}


int main(int argc, char **argv, char *env[])
{
	if (argc != 1)
		return 1;
	// dummy code to silent the warnings
	if (ft_strncmp(argv[0], "", 1) == 1)
		return 1;
	t_minishell *shell;

	shell = init_minishell();
	if (!shell)
		return (1);
	shell->env = init_mutable_env(env);
	if (!shell->env)
	{
		free_all(shell);
		/* clear local pointer so valgrind doesn’t see it on exit */
		shell = NULL;
		return (1);
	}
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, ctrl_backslash);
	if(runshell(shell) == -1)
		return (1);
	custom_save_history(shell);
	free_all(shell);
	/* avoid regaining pointer after free */
	shell = NULL;
	return (0);
}
