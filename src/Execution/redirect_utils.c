/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by haya              #+#    #+#             */
/*   Updated: 2026/03/28 21:07:13 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_has_ambiguous_target(t_redir_data *rd)
{
	if (!rd->filename)
		return (1);
	if (rd->filename[0] == '\0')
		return (1);
	return (0);
}

static int	print_ambiguous_redirect(void)
{
	ft_putstr_fd("minishell: ambiguous redirect\n", 2);
	return (-1);
}

int	redirect_input(t_redir_data *rd)
{
	int	fd;

	if (redir_has_ambiguous_target(rd))
		return (print_ambiguous_redirect());
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

	if (redir_has_ambiguous_target(rd))
		return (print_ambiguous_redirect());
	fd = open(rd->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
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

	if (redir_has_ambiguous_target(rd))
		return (print_ambiguous_redirect());
	fd = open(rd->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
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

// /*
// ** Walk the full pipeline tree and open+close every output redirect
// ** (DIR_OUT_TRUNC / DIR_OUT_APPEND) in the *parent* process before any fork.
// ** This guarantees the file exists on disk so that sibling pipeline segments
// ** that read those files with '<' never get ENOENT.
// ** (Mirrors bash's behaviour of opening redirect fds before forking children.)
// */
// void	precreate_output_redirects(t_tree *node)
// {
// 	t_list			*redir;
// 	t_redir_data	*rd;
// 	int				fd;
// 	int				flags;

// 	if (!node)
// 		return ;
// 	if (node->type == NODE_PIPE)
// 	{
// 		precreate_output_redirects(node->data.oper.left);
// 		precreate_output_redirects(node->data.oper.right);
// 		return ;
// 	}
// 	if (node->type == NODE_CMD)
// 		redir = node->data.cmd.redirections;
// 	else if (node->type == NODE_SUBSHELL)
// 		redir = node->data.subshell.redirections;
// 	else
// 		return ;
// 	while (redir)
// 	{
// 		rd = (t_redir_data *)redir->content;
// 		if ((rd->mode == DIR_OUT_TRUNC || rd->mode == DIR_OUT_APPEND)
// 			&& rd->filename && rd->filename[0])
// 		{
// 			if (rd->mode == DIR_OUT_TRUNC)
// 				flags = O_WRONLY | O_CREAT | O_TRUNC;
// 			else
// 				flags = O_WRONLY | O_CREAT | O_APPEND;
// 			fd = open(rd->filename, flags, 0666);
// 			if (fd != -1)
// 				close(fd);
// 		}
// 		redir = redir->next;
// 	}
// }

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
