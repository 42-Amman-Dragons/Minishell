/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:14:26 by haya              #+#    #+#             */
/*   Updated: 2026/03/01 11:47:08 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void wait_and_exit(pid_t pid)
{
    pid_t id;
    int err;
    int status;
    
    id = 0;
    err = 0;
    status = 0;
    while(1){
        id = waitpid(-1, &status, 0);
        if(id == pid)
            if (WIFEXITED(status))
                err = WEXITSTATUS(status);
        if(id <= 0)
            break;
    }
    exit(err);
}


static pid_t handle_left_pipe(int *fd, t_tree *node, char *env[])
{
    pid_t left_id;
    
    left_id = fork();
    if(left_id == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    else if(left_id == 0)
    {
        secure_close(fd[0]);
        if(dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            exit(1);
        }
        exec_tree(node->data.oper.left, env);
        secure_close(fd[1]);
        exit(0);
    }
    return(left_id);
}

static pid_t handle_right_pipe(int *fd, t_tree *node, char *env[])
{
    pid_t right_id;

    right_id = fork();
    if(right_id == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    else if(right_id == 0)
    {
        secure_close(fd[1]);
        if(dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            exit(1);
        }
        exec_tree(node->data.oper.right, env);
        secure_close(fd[0]);
        exit(0);
    }
    return(right_id);
}

void exec_pipe(t_tree *node, char *env[])
{
    int fd[2];
    pid_t right_id;
 
    right_id = 0;
    if(pipe(fd) == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    handle_left_pipe(fd, node, env);
    right_id = handle_right_pipe(fd, node, env);
    secure_close(fd[0]);
    secure_close(fd[1]);
    wait_and_exit(right_id);
}
