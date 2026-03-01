/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by haya              #+#    #+#             */
/*   Updated: 2026/03/01 17:20:45 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_tree(t_tree *node, char *env[])
{
    if(!node)
        return ;
    if(node->type == NODE_AND || node->type == NODE_OR)
    {
        exec_and_or(node, env);
        return ;
    }
    if(node->type ==  NODE_PIPE)
    {
        exec_pipe(node, env);
        return ;
    }
    exec_cmd(node,env);
}