/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_oper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:27:58 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:33:22 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_and_or(t_tree *node, t_minishell *shell)
{
	int	left_exit_code;
	int	right_exit_code;

	left_exit_code = exec_tree(node->data.oper.left, shell);
	shell->exit_status = left_exit_code;
	if (node->type == NODE_AND && left_exit_code != 0)
		return (left_exit_code);
	else if (node->type == NODE_OR && left_exit_code == 0)
		return (left_exit_code);
	right_exit_code = exec_tree(node->data.oper.right, shell);
	shell->exit_status = right_exit_code;
	return (right_exit_code);
}
