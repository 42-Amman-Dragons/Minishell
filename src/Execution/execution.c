/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by haya              #+#    #+#             */
/*   Updated: 2026/03/05 12:30:17 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_tree(t_tree *node, t_minishell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_AND || node->type == NODE_OR)
	{
		return exec_and_or(node, shell);
	}
	if (node->type == NODE_PIPE)
	{
		return exec_pipe(node, shell);
	}
	if (node->type == NODE_SUBSHELL)
	{
		return exec_subshell(node, shell);
	}
	return exec_cmd(node, shell);
}
