/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:14:26 by haya              #+#    #+#             */
/*   Updated: 2026/03/25 13:06:43 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			safe_close(int *fd, char *msg);

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
			err = child_exit_status(status);
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
		safe_close(&temp_std[0], "close error");
		safe_close(&temp_std[1], "close error");
		secure_close(fd[0], node, shell);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("DUP ERROR: ");
			free_and_exit(node, shell, 1);
		}
		secure_close(fd[1], node, shell);
		exec_tree(node->data.oper.left, shell);
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
		safe_close(&temp_std[0], "close error");
		safe_close(&temp_std[1], "close error");
		secure_close(fd[1], node, shell);
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("DUP ERROR: ");
			secure_close(fd[0], node, shell);
			free_and_exit(node, shell, 1);
		}
		secure_close(fd[0], node, shell);
		exec_tree(node->data.oper.right, shell);
		free_and_exit(node, shell, shell->exit_status);
	}
	return (right_id);
}

int	temp_redir(int *temp_stdin, int *temp_stdout)
{
	*temp_stdin = dup(STDIN_FILENO);
	*temp_stdout = dup(STDOUT_FILENO);
	if (*temp_stdin == -1 || *temp_stdout == -1)
	{
		perror("dup error");
		return (-1);
	}
	return (0);
}

void	safe_close(int *fd, char *msg)
{
	if ((*fd) == -1)
		return ;
	if (close(*fd) == -1)
		perror(msg);
	(*fd) = -1;
}

int	restore_close_redir(int *fd, int *temp_stdin, int *temp_stdout)
{
	safe_close(&fd[0], "close error");
	safe_close(&fd[1], "close error");
	if (dup2(*temp_stdin, STDIN_FILENO) == -1
		|| dup2(*temp_stdout, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		safe_close(temp_stdin, "close error");
		safe_close(temp_stdout, "close error");
		return (-1);
	}
	safe_close(temp_stdin, "close error");
	safe_close(temp_stdout, "close error");
	return (0);
}
int	exec_pipe(t_tree *node, t_minishell *shell)
{
	int		fd[2];
	pid_t	right_id;
	int		exit_code;
	int		temp_std[2];

	if (temp_redir(&temp_std[0], &temp_std[1]) == -1)
		return (-1);
	right_id = 0;
	exit_code = 0;
	if (pipe(fd) == -1)
	{
		perror("PIPE ERROR: ");
		safe_close(&temp_std[0], "close error");
		safe_close(&temp_std[1], "close error");
		return (1);
	}
	set_signals_exec();
	if (handle_left_pipe(fd, temp_std, node, shell) == -1)
	{
		if (restore_close_redir(fd, &temp_std[0], &temp_std[1]) == -1)
			free_and_exit(node, shell, 1);
		set_signals_prompt();
		return (1);
	}
	right_id = handle_right_pipe(fd, temp_std, node, shell);
	if (right_id == -1)
	{
		waitpid(-1, NULL, 0);
		if (restore_close_redir(fd, &temp_std[0], &temp_std[1]) == -1)
			free_and_exit(node, shell, 1);
		set_signals_prompt();
		return (1);
	}
	if (restore_close_redir(fd, &temp_std[0], &temp_std[1]) == -1)
		free_and_exit(node, shell, 1);
	exit_code = wait_all(right_id);
	set_signals_prompt();
	shell->exit_status = exit_code;
	return (exit_code);
}
