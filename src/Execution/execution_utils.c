/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:42:48 by haya              #+#    #+#             */
/*   Updated: 2026/03/01 14:04:16 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void secure_close(int fd)
{
    if(close(fd) == -1)
    {
        perror("Close file error");
        exit (1);
    }
}

void handle_redirections(t_list *redir)
{
    t_redir_data *redir_data;
    int fd;
    
    redir_data = NULL;
    fd = 0;
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
                exit (1);
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
                exit (1);
            }
            dup2(fd, STDOUT_FILENO);
        }
        secure_close(fd);
        redir = redir->next;
    }
}