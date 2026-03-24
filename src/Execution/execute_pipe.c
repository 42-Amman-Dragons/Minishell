/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:14:26 by haya              #+#    #+#             */
/*   Updated: 2026/03/24 00:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_all(pid_t pid)
{
	pid_t	id;
	int		err;
	int		status;

	id = 0;
	err = 0;
	status = 0;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id == pid)
		{
			if (WIFSIGNALED(status))
				err = 128 + WTERMSIG(status);
			else if (WIFEXITED(status))
				err = WEXITSTATUS(status);
		}
		if (id <= 0)
			break ;
	}
	return (err);
}

static pid_t	handle_left_pipe(int *fd, int *temp_std, t_tree *node,
		t_minishell *shell)
{
	pid_t	left_id;

	left_id = fork();
	if (left_id == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	else if (left_id == 0)
	{
		set_signals_child();
		close(temp_std[0]);
		close(temp_std[1]);
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("DUP ERROR: ");
			free_and_exit(node, shell, 1);
		}
		exec_tree(node->data.oper.left, shell);
		close(fd[1]);
		free_and_exit(node, shell, shell->exit_status);
	}
	return (left_id);
}

static pid_t	handle_right_pipe(int *fd, int *temp_std, t_tree *node,
		t_minishell *shell)
{
	pid_t	right_id;

	right_id = fork();
	if (right_id == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	else if (right_id == 0)
	{
		set_signals_child();
		close(temp_std[0]);
		close(temp_std[1]);
		close(fd[1]);
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("DUP ERROR: ");
			free_and_exit(node, shell, 1);
		}
		exec_tree(node->data.oper.right, shell);
		close(fd[0]);
		free_and_exit(node, shell, shell->exit_status);
	}
	return (right_id);
}

static int	fork_pipe_fail(int *fd, int *temp_std, t_tree *node,
		t_minishell *shell)
{
	if (restore_close_redir(fd, &temp_std[0], &temp_std[1]) == -1)
		free_and_exit(node, shell, 1);
	set_signals_prompt();
	return (1);
}

int	exec_pipe(t_tree *node, t_minishell *shell)
{
	int		fd[2];
	pid_t	right_id;
	int		temp_std[2];

	if (init_pipe(fd, temp_std) == -1)
		return (1);
	set_signals_exec();
	if (handle_left_pipe(fd, temp_std, node, shell) == -1)
		return (fork_pipe_fail(fd, temp_std, node, shell));
	right_id = handle_right_pipe(fd, temp_std, node, shell);
	if (right_id == -1)
	{
		waitpid(-1, NULL, 0);
		return (fork_pipe_fail(fd, temp_std, node, shell));
	}
	if (restore_close_redir(fd, &temp_std[0], &temp_std[1]) == -1)
		free_and_exit(node, shell, 1);
	shell->exit_status = wait_all(right_id);
	set_signals_prompt();
	return (shell->exit_status);
}
