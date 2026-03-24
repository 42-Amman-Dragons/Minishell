/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 05:28:07 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_input(t_redir_data *rd)
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

static int	redirect_output(t_redir_data *rd)
{
	int	fd;

	fd = open(rd->filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
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

static int	redirect_append(t_redir_data *rd)
{
	int	fd;

	fd = open(rd->filename, O_RDWR | O_CREAT | O_APPEND, 0644);
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

static int	redirect_heredoc(t_redir_data *rd)
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

static int	redirect(t_redir_data *rd)
{
	if (rd->mode == DIR_IN_FILE)
	{
		if (redirect_input(rd) == -1)
			return (-1);
	}
	else if (rd->mode == DIR_OUT_TRUNC)
	{
		if (redirect_output(rd) == -1)
			return (-1);
	}
	else if (rd->mode == DIR_IN_HEREDOC)
	{
		if (redirect_heredoc(rd) == -1)
			return (-1);
	}
	else if (rd->mode == DIR_OUT_APPEND)
	{
		if (redirect_append(rd) == -1)
			return (-1);
	}
	return (0);
}

int	handle_redirections(t_tree *node)
{
	t_list			*redir;
	t_redir_data	*rd;

	redir = NULL;
	if (node->type == NODE_CMD)
		redir = node->data.cmd.redirections;
	else if (node->type == NODE_SUBSHELL)
		redir = node->data.subshell.redirections;
	while (redir)
	{
		rd = (t_redir_data *)redir->content;
		if (redirect(rd) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
