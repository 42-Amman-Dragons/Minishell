/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by haya              #+#    #+#             */
/*   Updated: 2026/02/15 12:55:29 by haya             ###   ########.fr       */
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
void exec_pipe(t_tree *node, char *env[],int *err, pid_t *last_id)
{
    int fd[2];
    int id;
    int temp_stdout;
    int temp_stdin;
    
    id = 0;
    temp_stdin= 0;
    temp_stdout = 0;
    dup2(temp_stdin, STDIN_FILENO);
    dup2(temp_stdout, STDOUT_FILENO);
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
        exec_tree(node->right, env,err, last_id);
        close(fd[0]);
        dup2(0, temp_stdin);
    }
    else{
        close(fd[0]);
        if(ft_strncmp(node->right->type, "PIPE", 4) != 0)
        {
            // printf("last cmd:%s\n", node->right->argumnts[0]);
            *last_id = id;
        }
        if(dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            *err = 1;
        }
        exec_tree(node->left, env,err, last_id);
        close(fd[1]);
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
        ft_putstr_fd("Command not found\n", 2);
        *err = 1;
        exit(*err);
    }
    if(execve(node->argumnts[0], node->argumnts, env) == -1)
    {
        perror("EXECVE ERROR: ");
        exit(*err);
    }
}

void execute(t_tree *node, char *env[],int *err)
{
    pid_t id;
    pid_t last_id;
    int status;

    id = 0;
    last_id = -2;
    exec_tree(node, env, err, &last_id);
    while(1){
        id = waitpid(-1, &status, 0);
        if(id == last_id)
            if (WIFEXITED(status))
                *err = WEXITSTATUS(status);
        if(id <= 0)
            break;
    }
    //@TODO: Check if anything need to be freed or closed here.
    exit(*err);
}
void exec_tree(t_tree *node, char *env[],int *err, pid_t *last_id)
{
    if(!node)
        return ;
    // if(*err == 1)
    //     return ;
    if(ft_strncmp(node->type, "PIPE", 4)== 0)
        exec_pipe(node, env, err, last_id);
    exec_cmd(node,env, err);   
    // exec_redir(tokens, "DIR_IN");
    // exec_redir(tokens, "DIR_OUT");
}