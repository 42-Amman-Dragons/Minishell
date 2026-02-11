/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by haya              #+#    #+#             */
/*   Updated: 2026/02/11 14:08:46 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// void exec_and()
// {
    
// }
// void exec_or()
// {
    
// }
/*
@TODO: Should the close have a close error with perror ?
*/
void exec_pipe(t_tree *node, char *env[],int *err)
{
    int fd[2];
    int id;
    
    id = 0;
    if(pipe(fd) == -1)
    {
        perror("PIPE ERROR: ");
        *err = 1;
    }
    id = fork();
    if(id == -1)
    {
        perror("PIPE ERROR: ");
        *err = 1;
    }
    else if(id == 0)
    {
        close(fd[1]);
        if(dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            *err = 1;
        }
        exec_tree(node->right, env,err);
    }
    else{
        close(fd[0]);
        if(dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            *err = 1;
        }
        exec_tree(node->left, env,err);
    }
}

// void exec_redir(t_tree *node)
// {
//     if(node->type)
// }

void exec_cmd(t_tree *node, char *env[], int *err)
{
    if(!node->argumnts)
    {
        ft_putstr_fd("Command not found", 2);
        *err = 1;
        return;
    }
    if(execve(node->argumnts[0], node->argumnts, env) == -1)
    {
        perror("EXECVE ERROR: ");
        *err = 1;   
    }
}

void exec_tree(t_tree *node, char *env[],int *err)
{
    if(!node)
        return ;
    if(*err == 1)
        return ;
    if(ft_strncmp(node->type, "PIPE", 4)== 0)
        exec_pipe(node, env, err);
    exec_cmd(node,env, err);   
    // exec_redir(tokens, "DIR_IN");
    // exec_redir(tokens, "DIR_OUT");
}