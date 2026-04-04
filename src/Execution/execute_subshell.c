/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 10:07:01 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 12:07:21 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_subshell_child(t_tree *node, t_minishell *shell)
{
	shell->is_child = 1;
	set_signals_child();
	expander(node, shell);
	if (handle_redirections(node) == -1)
		free_and_exit(node, shell, 1);
	exec_tree(node->data.subshell.child, shell);
	free_and_exit(node, shell, shell->exit_status);
}

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
		run_subshell_child(node, shell);
	waitpid(pid, &status, 0);
	print_sigquit_if_needed(status, shell);
	shell->exit_status = child_exit_status(status);
	set_signals_prompt();
	return (shell->exit_status);
}
