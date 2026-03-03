/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:42:48 by haya              #+#    #+#             */
/*   Updated: 2026/03/03 13:03:53 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void secure_close(int fd,t_tree *node ,t_minishell *shell)
{
    if(close(fd) == -1)
    {
        perror("Close file error");
        free_and_exit(node, shell, 1);
    }
}

void free_and_exit(t_tree *node, t_minishell *shell, int exit_code)
{
    free_tree(node);
    free_all(shell);
    exit(exit_code);
}

void handle_redirections(t_tree *node, t_minishell *shell)
{
    t_list *redir;
    t_redir_data *redir_data;
    int fd;
    
    redir = NULL;
    redir_data = NULL;
    fd = 0;
    if(node->type == NODE_CMD)
        redir = node->data.cmd.redirections;
    else if (node->type == NODE_SUBSHELL)
        redir = node->data.subshell.redirections;
    while(redir)
    {
        redir_data = (t_redir_data *)redir->content;        
        if (redir_data->mode == DIR_IN_FILE)
        {
            if(access(redir_data->filename, R_OK) != 0)
            {
                ft_putstr_fd(redir_data->filename, 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                return;
            }
            fd = open(redir_data->filename, O_RDONLY);
            dup2(fd, STDIN_FILENO);
        }
        if (redir_data->mode == DIR_OUT_TRUNC)
        {
            fd = open(redir_data->filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
            if(fd == -1)
            {
                perror(redir_data->filename);
                free_and_exit(node, shell, 1);
            }
            dup2(fd, STDOUT_FILENO);
        }
        if (redir_data->mode == DIR_IN_HEREDOC)
        {
            dup2(redir_data->heredoc_fd, STDIN_FILENO);
        }
        if (redir_data->mode == DIR_OUT_APPEND)
        {
            fd = open(redir_data->filename, O_RDWR | O_CREAT | O_APPEND, 0644);
            if(fd == -1)
            {
                perror(redir_data->filename);
                free_and_exit(node, shell, 1);
            }
            dup2(fd, STDOUT_FILENO);
        }
        secure_close(fd, node, shell);
        redir = redir->next;
    }
}
