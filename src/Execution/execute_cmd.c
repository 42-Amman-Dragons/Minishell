/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/27 04:34:46 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_cmd_error(char *cmd_name, t_tree *node, t_minishell *shell)
{
	int	exit_code;

	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	if (!node->data.cmd.args[0])
	{
		ft_putstr_fd("command not found\n", 2);
		exit_code = 127;
	}
	else if (is_command_a_directory(node->data.cmd.args[0]))
	{
		ft_putstr_fd("Is a directory\n", 2);
		exit_code = 126;
	}
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		if (errno == ENOENT)
			exit_code = 127;
		else
			exit_code = 126;
	}
	free(cmd_name);
	free_and_exit(node, shell, exit_code);
}

void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;

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
		free(node->data.cmd.args[0]);
		node->data.cmd.args[0] = absoulute_path(cmd_name, shell->env);
	}
	if (!node->data.cmd.args[0] || execve(node->data.cmd.args[0],
			node->data.cmd.args, shell->env) == -1)
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
	shell->exit_status = child_exit_status(status);
	return (shell->exit_status);
}

int	handle_builtin(int idx, t_tree *node, t_minishell *shell)
{
	if (temp_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout) == -1)
		return (shell->exit_status = 1);
	track_fd(shell, &shell->builtin_temp_stdin);
	track_fd(shell, &shell->builtin_temp_stdout);
	if (handle_redirections(node) == -1)
	{
		restore_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout);
		return (shell->exit_status = 1);
	}
	shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
	if (restore_redir(&shell->builtin_temp_stdin, &shell->builtin_temp_stdout) == -1)
		shell->exit_status = 1;
	return (shell->exit_status);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	idx;

	if (node->data.cmd.args)
	{
		idx = is_builtin(node->data.cmd.args[0]);
		if (idx >= 0)
			return (handle_builtin(idx, node, shell));
	}
	return (handle_external_cmd(shell, node));
}
