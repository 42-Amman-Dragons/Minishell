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

// Returns 1 if path is a directory, 0 otherwise
static int	is_command_a_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

void	execve_cmd(t_tree *node, t_minishell *shell)
{
	char	*cmd_name;

	if (handle_redirections(node, shell) == -1)
		free_and_exit(node, shell, 1);
	if (!node->data.cmd.args)
		free_and_exit(node, shell, 0);
	cmd_name = ft_strdup(node->data.cmd.args[0]);
	if (!ft_strchr(node->data.cmd.args[0], '/'))
	{
		free(node->data.cmd.args[0]);
		node->data.cmd.args[0] = absoulute_path(cmd_name, shell->env);
	}
	if (!node->data.cmd.args[0])
		cmd_not_found(cmd_name, node, shell);
	// Check before execve: on Linux execve() returns EACCES for directories,
	// not EISDIR. Detect it early and print the correct error message.
	if (is_command_a_directory(node->data.cmd.args[0]))
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(cmd_name);
		free_and_exit(node, shell, 126);
	}
	if (execve(node->data.cmd.args[0], node->data.cmd.args, shell->env) == -1)
	{
		// ENOENT = file/path not found -> 127 (command not found)
		// EACCES = no execute permission -> 126
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(cmd_name);
		if (errno == ENOENT)
			free_and_exit(node, shell, 127);
		free_and_exit(node, shell, 126);
	}
}

int	temp_dup_error(int temp_stdin, int temp_stdout, t_minishell *shell)
{
	if (temp_stdin != -1)
		close(temp_stdin);
	if (temp_stdout != -1)
		close(temp_stdout);
	ft_putstr_fd("minishell: dup error\n", 2); // I have added this
	shell->exit_status = 1;
	return (1);
}

int	redirection_failure(int temp_stdin, int temp_stdout, t_minishell *shell)
{
	dup2(temp_stdin, STDIN_FILENO);
	dup2(temp_stdout, STDOUT_FILENO);
	close(temp_stdin);
	close(temp_stdout);
	shell->exit_status = 1;
	return (1);
}

int	run_cmd(t_minishell *shell, t_tree *node)
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
	if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (shell->exit_status);
}

int	exec_cmd(t_tree *node, t_minishell *shell)
{
	int	idx;
	int	temp_stdin;
	int	temp_stdout;

	idx = -1;
	if (node->data.cmd.args)
	{
		idx = is_builtin(node->data.cmd.args[0]);
		if (idx >= 0)
		{
			temp_stdin = dup(STDIN_FILENO);
			temp_stdout = dup(STDOUT_FILENO);
			if (temp_stdin == -1 || temp_stdout == -1)
				return (temp_dup_error(temp_stdin, temp_stdout, shell));
			if (handle_redirections(node, shell) == -1)
				return (redirection_failure(temp_stdin, temp_stdout, shell));
			shell->exit_status = call_builtin(idx, node->data.cmd.args, shell);
			dup2(temp_stdin, STDIN_FILENO);
			dup2(temp_stdout, STDOUT_FILENO);
			close(temp_stdin);
			close(temp_stdout);
			return (shell->exit_status);
		}
	}
	return (run_cmd(shell, node));
}
