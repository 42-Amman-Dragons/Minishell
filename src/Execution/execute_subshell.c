/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 10:07:01 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 01:09:45 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_subshell(t_tree *node, t_minishell *shell)
{
	pid_t	pid;
	int		status;

	set_signals_exec();
	pid = fork();
	status = 0;
	if (pid == -1)
	{
		perror("Fork Error");
		set_signals_prompt();
		return (1);
	}
	if (pid == 0)
	{
		set_signals_child();
		expander(node, shell);
		if (handle_redirections(node) == -1)
			free_and_exit(node, shell, 1);
		exec_tree(node->data.subshell.child, shell);
		free_and_exit(node, shell, shell->exit_status);
	}
	waitpid(pid, &status, 0);
	shell->exit_status = child_exit_status(status);
	set_signals_prompt();
	return (shell->exit_status);
}
