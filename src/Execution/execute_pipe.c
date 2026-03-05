/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:14:26 by haya              #+#    #+#             */
/*   Updated: 2026/03/05 12:32:30 by haya             ###   ########.fr       */
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
			if (WIFEXITED(status))
				err = WEXITSTATUS(status);
		if (id <= 0)
			break ;
	}
	return (err);
}

static pid_t	handle_left_pipe(int *fd, t_tree *node, t_minishell *shell)
{
	pid_t	left_id;

	left_id = fork();
	if (left_id == -1)
	{
		perror("PIPE ERROR: ");
		free_and_exit(node, shell, 1);
	}
	else if (left_id == 0)
	{
		secure_close(fd[0], node, shell);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("DUP ERROR: ");
			free_and_exit(node, shell, 1);
		}
		exec_tree(node->data.oper.left, shell);
		secure_close(fd[1], node, shell);
		free_and_exit(node, shell, 0);
	}
	return (left_id);
}

static pid_t	handle_right_pipe(int *fd, t_tree *node, t_minishell *shell)
{
	pid_t	right_id;

	right_id = fork();
	if (right_id == -1)
	{
		perror("PIPE ERROR: ");
		free_and_exit(node, shell, 1);
	}
	else if (right_id == 0)
	{
		secure_close(fd[1], node, shell);
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("DUP ERROR: ");
			free_and_exit(node, shell, 1);
		}
		exec_tree(node->data.oper.right, shell);
		secure_close(fd[0], node, shell);
		free_and_exit(node, shell, 0);
	}
	return (right_id);
}

int	exec_pipe(t_tree *node, t_minishell *shell)
{
	int		fd[2];
	pid_t	right_id;
	int		exit_code;
	int temp_stdin;
	int temp_stdout;

	temp_stdin = STDIN_FILENO;
	temp_stdout = STDOUT_FILENO;
	right_id = 0;
	exit_code = 0;
	if (pipe(fd) == -1)
	{
		perror("PIPE ERROR: ");
		return (1);
	}
	handle_left_pipe(fd, node, shell);
	right_id = handle_right_pipe(fd, node, shell);
	close(fd[0]);
	close(fd[1]);
	dup2(STDIN_FILENO, temp_stdin);
	dup2(STDOUT_FILENO, temp_stdout);
	exit_code = wait_all(right_id);
	return(exit_code);
}
