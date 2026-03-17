/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 10:07:01 by haya              #+#    #+#             */
/*   Updated: 2026/03/17 05:27:59 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_exit_code(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	exec_subshell(t_tree *node, t_minishell *shell)
{
	pid_t	pid;
	int		exit_code;

	set_signals_exec();
	pid = fork();
	exit_code = 0;
	if (pid == -1)
	{
		perror("Fork Error");
		set_signals_prompt();
		return (1);
	}
	if (pid == 0)
	{
		set_signals_child();
		if (handle_redirections(node, shell) == -1)
			free_and_exit(node, shell, 1);
		exec_tree(node->data.subshell.child, shell);
		free_and_exit(node, shell, shell->exit_status);
	}
	exit_code = wait_exit_code(pid);
	shell->exit_status = exit_code;
	set_signals_prompt();
	return (exit_code);
}
