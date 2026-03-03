/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_oper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:27:58 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 12:52:10 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handel_left_oper(t_tree *node,t_minishell *shell)
{
    pid_t left_id;
    int status;
    int left_exit_code;

    left_exit_code = 0;
    left_id = fork();
    status = 0;

    if (left_id == -1)
    {
        perror("PIPE ERROR: ");
        free_and_exit(node, shell, 1);
    }
    else if (left_id == 0)
    {
        exec_tree(node->data.oper.left, shell);
        free_and_exit(node, shell, 0);
    }
    waitpid(left_id, &status, 0);
    if (WIFEXITED(status))
        left_exit_code = WEXITSTATUS(status);
    return (left_exit_code);
}

static int handle_right_oper(t_tree *node,t_minishell *shell)
{
    pid_t right_id;
    int status;
    int right_exit_code;

    right_id = fork();
    if (right_id == -1)
    {
        perror("PIPE ERROR: ");
        free_and_exit(node, shell, 1);
    }
    else if (right_id == 0)
    {
        exec_tree(node->data.oper.right, shell);
        free_and_exit(node, shell, 0);
    }
    waitpid(right_id, &status, 0);
    if (WIFEXITED(status))
        right_exit_code = WEXITSTATUS(status);
    return (right_exit_code);
}

void exec_and_or(t_tree *node,t_minishell *shell)
{
    int left_exit_code;
    int right_exit_code;

    left_exit_code = handel_left_oper(node, shell);
    if (node->type == NODE_AND && left_exit_code != 0)
        free_and_exit(node, shell, left_exit_code);
    else if (node->type == NODE_OR && left_exit_code == 0)
        free_and_exit(node, shell, left_exit_code);
    right_exit_code = handle_right_oper(node, shell);
    free_and_exit(node, shell, right_exit_code);
}