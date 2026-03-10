/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/08 16:56:24 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_input(t_redir_data *rd, t_tree *node, t_minishell *shell)
{
	int	fd;

	(void)node;
	(void)shell;
	fd = open(rd->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(rd->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	redirect_output(t_redir_data *rd, t_tree *node, t_minishell *shell)
{
	int	fd;

	(void)node;
	(void)shell;
	fd = open(rd->filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(rd->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	redirect_append(t_redir_data *rd, t_tree *node, t_minishell *shell)
{
	int	fd;

	(void)node;
	(void)shell;
	fd = open(rd->filename, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(rd->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	redirect_heredoc(t_redir_data *rd)
{
	dup2(rd->heredoc_fd, STDIN_FILENO);
	close(rd->heredoc_fd);
	return (0);
}

int		handle_redirections(t_tree *node, t_minishell *shell)
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
		if (rd->mode == DIR_IN_FILE)
		{
			if (redirect_input(rd, node, shell) == -1)
				return (-1);
		}
		else if (rd->mode == DIR_OUT_TRUNC)
		{
			if (redirect_output(rd, node, shell) == -1)
				return (-1);
		}
		else if (rd->mode == DIR_IN_HEREDOC)
			redirect_heredoc(rd);
		else if (rd->mode == DIR_OUT_APPEND)
		{
			if (redirect_append(rd, node, shell) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
