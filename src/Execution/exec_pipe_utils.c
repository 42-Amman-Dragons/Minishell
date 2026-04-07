/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:49:01 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_all(pid_t pid, t_minishell *shell)
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
			err = child_exit_status(status);
			print_sigquit_if_needed(status, shell);
		}
		if (id <= 0)
			break ;
	}
	return (err);
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

int	restore_redir(int *temp_stdin, int *temp_stdout)
{
	if (dup2(*temp_stdin, STDIN_FILENO) == -1 || dup2(*temp_stdout,
													  STDOUT_FILENO) == -1)
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

int	restore_close_redir(int *fd, int *temp_stdin, int *temp_stdout)
{
	safe_close(&fd[0], "close error");
	safe_close(&fd[1], "close error");
	return (restore_redir(temp_stdin, temp_stdout));
}
