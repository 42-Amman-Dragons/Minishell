/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 10:58:39 by haya              #+#    #+#             */
/*   Updated: 2026/03/21 02:47:03 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_cmd_error(char *cmd_name, t_tree *node, t_minishell *shell)
{
	int	exit_code;

	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	if (!node->data.cmd.args[0])
		ft_putstr_fd("command not found\n", 2);
	else if (is_command_a_directory(node->data.cmd.args[0]))
		ft_putstr_fd("Is a directory\n", 2);
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	if (!node->data.cmd.args[0] || errno == ENOENT)
		exit_code = 127;
	else
		exit_code = 126;
	free(cmd_name);
	free_and_exit(node, shell, exit_code);
}

void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;

	if (handle_redirections(node) == -1)
		free_and_exit(node, shell, 1);
	if (!node->data.cmd.args)
		free_and_exit(node, shell, 0);
	cmd_name = ft_strdup(node->data.cmd.args[0]);
	if (!cmd_name)
		free_and_exit(node, shell, 1);
	if (!ft_strchr(node->data.cmd.args[0], '/'))
	{
		free(node->data.cmd.args[0]);
		node->data.cmd.args[0] = absolute_path(cmd_name, shell->env);
	}
	if (!node->data.cmd.args[0])
		handle_cmd_error(cmd_name, node, shell);
	execve(node->data.cmd.args[0], node->data.cmd.args, shell->env);
	handle_cmd_error(cmd_name, node, shell);
}

static int	run_builtin(int idx, int *temp_std,
		t_tree *node, t_minishell *shell)
{
	if (handle_redirections(node) == -1)
	{
		if (restore_stdio(temp_std[0], temp_std[1]) == -1)
			free_and_exit(node, shell, 1);
		shell->exit_status = 1;
		return (1);
	}
	shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
	if (restore_stdio(temp_std[0], temp_std[1]) == -1)
		free_and_exit(node, shell, 1);
	return (shell->exit_status);
}

int	run_cmd(t_tree *node, t_minishell *shell)
{
	int	id;
	int	status;

	set_signals_exec();
	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		set_signals_prompt();
		return (errno);
	}
	if (id == 0)
	{
		set_signals_child();
		execve_cmd(node, shell);
	}
	close_heredoc_fds(node);
	waitpid(id, &status, 0);
	set_signals_prompt();
	handle_child_exit_status(status, shell);
	return (shell->exit_status);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	idx;
	int	temp_std[2];

	if (!node->data.cmd.args)
		return (run_cmd(node, shell));
	idx = is_builtin(node->data.cmd.args[0]);
	if (idx < 0)
		return (run_cmd(node, shell));
	if (temp_redir(&temp_std[0], &temp_std[1]) == -1)
	{
		shell->exit_status = 1;
		return (1);
	}
	return (run_builtin(idx, temp_std, node, shell));
}
