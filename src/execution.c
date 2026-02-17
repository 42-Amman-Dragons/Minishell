/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:38:10 by haya              #+#    #+#             */
/*   Updated: 2026/02/17 14:05:29 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void exec_and_or(t_tree *node, char *env[])
{
    pid_t left_id;
    pid_t right_id;
    int     status;
    int left_exit_code;
    
    right_id = 0;
    status = 0;
    left_exit_code = 0;
    left_id = fork();
    if(left_id == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    else if(left_id == 0)
    {
        exec_tree(node->left, env);
        exit(0);
    }
    waitpid(left_id, &status, 0);
    if(WIFEXITED(status))
        left_exit_code = WEXITSTATUS(status);
    if(ft_strcmp(node->type, "AND") == 0 && left_exit_code != 0)
        exit(left_exit_code);
    else if(ft_strcmp(node->type, "OR") == 0 && left_exit_code == 0)
        exit(left_exit_code);
    right_id = fork();
    if(right_id == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    else if(right_id == 0)
    {
        exec_tree(node->right, env);
        exit(0);
    }
    waitpid(right_id, &status, 0);
    if(WIFEXITED(status))
        exit(WEXITSTATUS(status));
    exit(0);
}
/*
@TODO: Should the close have a close error with perror ?
*/


void wait_and_exit(pid_t right_id)
{
    pid_t id;
    int err;
    int status;
    
    id = 0;
    err = 0;
    status = 0;
    while(1){
        id = waitpid(-1, &status, 0);
        printf("id:%d!\n", id);
        if(id == right_id)
            if (WIFEXITED(status))
                err = WEXITSTATUS(status);
        if(id <= 0)
            break;
    }
    exit(err);
}

void exec_pipe(t_tree *node, char *env[])
{
    int fd[2];
    pid_t left_id;
    pid_t right_id;
    pid_t last_id;
 
    left_id = 0;
    right_id = 0;

    if(pipe(fd) == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    right_id = fork();
    if(right_id == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    else if(right_id == 0)
    {
        close(fd[1]);
        if(dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            exit(1);
        }
        exec_tree(node->right, env);
        close(fd[0]);
        exit(0);
    }
    if(ft_strncmp(node->right->type, "PIPE", 4) != 0)
    {    
        last_id = right_id;
        printf("last id: %d\n", last_id);
    }
    left_id = fork();
    if(left_id == -1)
    {
        perror("PIPE ERROR: ");
        exit(1);
    }
    else if(left_id == 0)
    {
        close(fd[0]);
        if(dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("DUP ERROR: ");
            exit(1);
        }
        exec_tree(node->left, env);
        close(fd[1]);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    wait_and_exit(right_id);
}

void exec_redir(t_tree *node, char *env[])
{
    int fd;
    if(ft_strcmp(node->type, "DIR_IN") == 0)
    {
        if(access(node->filename, R_OK) != 0)
        {
            ft_putstr_fd(node->filename, 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            return;
        }
        fd = open(node->filename, O_RDONLY);
        dup2(fd, STDIN_FILENO);
    }
    if(ft_strcmp(node->type, "DIR_OUT") == 0)
    {
        fd = open(node->filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if(fd == -1)
        {
            perror(node->filename);
            exit (1);
        }
        dup2(fd, STDOUT_FILENO);
    }
    exec_tree(node->left, env);
}

void exec_cmd(t_tree *node, char *env[])
{
    if(!node->argumnts)
    {
        ft_putstr_fd("Command not found\n", 2);
        exit(1);
    }
    if(execve(node->argumnts[0], node->argumnts, env) == -1)
    {
        perror("EXECVE ERROR: ");
        exit(1);
    }
    exit(0);
}

void exec_tree(t_tree *node, char *env[])
{
    if(!node)
        return ;
    if(ft_strcmp(node->type, "AND")== 0)
    {
        exec_and_or(node, env);
        return ;
    }
    if(ft_strcmp(node->type, "OR")== 0)
    {
        exec_and_or(node, env);
        return ;
    }
    if(ft_strcmp(node->type, "PIPE")== 0)
    {
        exec_pipe(node, env);
        return ;
    }
    if(ft_strcmp(node->type, "DIR_IN") == 0 || ft_strcmp(node->type, "DIR_OUT") == 0)
    {
        exec_redir(node, env);
        return ;
    }
    exec_cmd(node,env);
}