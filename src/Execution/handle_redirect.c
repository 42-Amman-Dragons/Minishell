/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/25 11:11:07 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
