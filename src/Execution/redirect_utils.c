/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/27 00:00:00 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_input(t_redir_data *rd)
{
	int	fd;

	fd = open(rd->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(rd->filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_output(t_redir_data *rd)
{
	int	fd;

	fd = open(rd->filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(rd->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_append(t_redir_data *rd)
{
	int	fd;

	fd = open(rd->filename, O_RDWR | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(rd->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_heredoc(t_redir_data *rd)
{
	if (dup2(rd->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(rd->heredoc_fd);
		rd->heredoc_fd = -1;
		return (-1);
	}
	close(rd->heredoc_fd);
	rd->heredoc_fd = -1;
	return (0);
}
