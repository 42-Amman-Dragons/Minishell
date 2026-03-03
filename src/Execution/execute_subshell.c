/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 10:07:01 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 12:55:21 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int wait_exit_code(pid_t pid)
{
    int status;

    status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return (0);
}

void exec_subshell(t_tree *node,t_minishell *shell)
{
    pid_t pid;
    int exit_code;

    pid = fork();
    exit_code = 0;
    if (pid == -1)
    {
        perror("Fork Error");
        free_and_exit(node, shell, 1);
    }
    if (pid == 0)
    {
        handle_redirections(node, shell);
        exec_tree(node->data.subshell.child, shell);
        free_and_exit(node, shell, 0); // Most likely a dead code.
    }
    exit_code = wait_exit_code(pid);
    free_and_exit(node, shell, exit_code);
}
