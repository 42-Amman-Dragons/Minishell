/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:28:21 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int *fd, char *msg)
{
	if ((*fd) == -1)
		return ;
	if (close(*fd) == -1)
		perror(msg);
	(*fd) = -1;
}

void	secure_close(int fd, t_tree *node, t_minishell *shell)
{
	if (fd == -1)
		return ;
	if (close(fd) == -1)
	{
		perror("Close file error");
		free_and_exit(node, shell, 1);
	}
}

int	pipe_error_and_close(int *temp_std)
{
	perror("PIPE ERROR: ");
	safe_close(&temp_std[0], "close error");
	safe_close(&temp_std[1], "close error");
	return (1);
}
