/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 11:08:07 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_tree(t_tree *node, t_minishell *shell)
{
    if (!node)
        return;
    if (node->type == NODE_AND || node->type == NODE_OR)
    {
        exec_and_or(node, shell);
        return;
    }
    if (node->type == NODE_PIPE)
    {
        exec_pipe(node, shell);
        return;
    }
    if (node->type == NODE_SUBSHELL)
    {
        exec_subshell(node, shell);
        return;
    }
    exec_cmd(node, shell);
}