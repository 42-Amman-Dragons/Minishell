/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:30:08 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;
	char	*resolved;

	if (handle_redirections(node) == -1)
		free_and_exit(node, shell, 1);
	close_tracked_fds(shell);
	if (!node->data.cmd.args)
		free_and_exit(node, shell, 0);
	cmd_name = ft_strdup(node->data.cmd.args[0]);
	if (!cmd_name)
		free_and_exit(node, shell, 1);
	if (!ft_strchr(node->data.cmd.args[0], '/'))
	{
		resolved = absoulute_path(cmd_name, shell->env);
		if (!resolved)
			handle_cmd_error(cmd_name, node, shell);
		free(node->data.cmd.args[0]);
		node->data.cmd.args[0] = resolved;
	}
	if (execve(node->data.cmd.args[0], node->data.cmd.args, shell->env) == -1)
		handle_cmd_error(cmd_name, node, shell);
}

int	handle_external_cmd(t_minishell *shell, t_tree *node)
{
	int	id;
	int	status;

	set_signals_exec();
	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		set_signals_prompt();
		return (1);
	}
	if (id == 0)
	{
		set_signals_child();
		execve_cmd(node, shell);
	}
	waitpid(id, &status, 0);
	set_signals_prompt();
	print_sigquit_if_needed(status, shell);
	shell->exit_status = child_exit_status(status);
	return (shell->exit_status);
}

int	handle_builtin(int idx, t_tree *node, t_minishell *shell)
{
	if (temp_redir(&shell->builtin_temp_stdin,
			&shell->builtin_temp_stdout) == -1)
		return (shell->exit_status = 1);
	track_fd(shell, &shell->builtin_temp_stdin);
	track_fd(shell, &shell->builtin_temp_stdout);
	if (handle_redirections(node) == -1)
	{
		restore_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout);
		return (shell->exit_status = 1);
	}
	shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
	if (restore_redir(&shell->builtin_temp_stdin,
			&shell->builtin_temp_stdout) == -1)
		shell->exit_status = 1;
	return (shell->exit_status);
}

static int	handle_redir_only_cmd(t_tree *node, t_minishell *shell)
{
	if (temp_redir(&shell->builtin_temp_stdin,
			&shell->builtin_temp_stdout) == -1)
		return (shell->exit_status = 1);
	if (handle_redirections(node) == -1)
	{
		restore_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout);
		return (shell->exit_status = 1);
	}
	if (restore_redir(&shell->builtin_temp_stdin,
			&shell->builtin_temp_stdout) == -1)
		return (shell->exit_status = 1);
	return (shell->exit_status = 0);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	idx;

	expander(node, shell);
	if (!node->data.cmd.args)
		return (handle_redir_only_cmd(node, shell));
	if (node->data.cmd.args)
	{
		update_underscore_var(node, shell);
		idx = is_builtin(node->data.cmd.args[0]);
		if (idx >= 0)
			return (handle_builtin(idx, node, shell));
	}
	return (handle_external_cmd(shell, node));
}
