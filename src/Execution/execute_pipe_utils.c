/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 00:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	temp_redir(int *temp_stdin, int *temp_stdout)
{
	*temp_stdin = dup(STDIN_FILENO);
	*temp_stdout = dup(STDOUT_FILENO);
	if (*temp_stdin == -1 || *temp_stdout == -1)
	{
		perror("dup error");
		if (*temp_stdin != -1)
			close(*temp_stdin);
		if (*temp_stdout != -1)
			close(*temp_stdout);
		return (-1);
	}
	return (0);
}

int	restore_close_redir(int *fd, int *temp_stdin, int *temp_stdout)
{
	close(fd[0]);
	close(fd[1]);
	return (restore_stdio(*temp_stdin, *temp_stdout));
}

int	init_pipe(int *fd, int *temp_std)
{
	if (temp_redir(&temp_std[0], &temp_std[1]) == -1)
		return (-1);
	if (pipe(fd) == -1)
	{
		perror("PIPE ERROR: ");
		close(temp_std[0]);
		close(temp_std[1]);
		return (-1);
	}
	return (0);
}
