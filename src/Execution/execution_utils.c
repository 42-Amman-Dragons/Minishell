/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:42:48 by haya              #+#    #+#             */
/*   Updated: 2026/03/17 05:28:01 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_fds(t_tree *node)
{
	t_list			*redir;
	t_redir_data	*rd;

	if (node->type == NODE_CMD)
		redir = node->data.cmd.redirections;
	else if (node->type == NODE_SUBSHELL)
		redir = node->data.subshell.redirections;
	else
		return ;
	while (redir)
	{
		rd = (t_redir_data *)redir->content;
		if (rd->heredoc_fd >= 0)
		{
			close(rd->heredoc_fd);
			rd->heredoc_fd = -1;
		}
		redir = redir->next;
	}
}

void	secure_close(int fd, t_tree *node, t_minishell *shell)
{
	if (close(fd) == -1)
	{
		perror("Close file error");
		free_and_exit(node, shell, 1);
	}
}

void	free_and_exit(t_tree *node, t_minishell *shell, int exit_code)
{
	free_tree(node);
	exit(cleanup_shell(shell, exit_code));
}

void	free_splitted(char **splitted)
{
	int	i;

	if (!splitted)
		return ;
	i = 0;
	while (splitted[i])
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}
