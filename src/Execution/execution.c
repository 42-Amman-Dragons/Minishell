/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare <mabuqare@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:39:44 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_tree(t_tree *node, t_minishell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_AND || node->type == NODE_OR)
	{
		return (exec_and_or(node, shell));
	}
	if (node->type == NODE_PIPE)
	{
		return (exec_pipe(node, shell));
	}
	if (node->type == NODE_SUBSHELL)
	{
		return (exec_subshell(node, shell));
	}
	return (exec_cmd(node, shell));
}
