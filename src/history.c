/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:57:32 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/01/27 14:57:45 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int add_to_history(char *line, t_list **history)
{
    t_list *node;

    add_history(line);
    node = ft_lstnew(line);
    if(!node)
        return(0);
    ft_lstadd_back(history, node);
    return(1);
}

void load_history(t_list **history)
{
    int fd;
    char *line;

    fd = open("./history", O_RDWR | O_CREAT, S_IRWXU);
    if (access("./history", F_OK) != 0)
        ft_putstr_fd("cannot find history file\n", 2);
    if (access("./history", R_OK) != 0)
        ft_putstr_fd("read permission denied in the history\n", 2);
    if (access("./history", W_OK) != 0)
        ft_putstr_fd("write permission denied in the history\n", 2);
    while (1)
    {
            line = get_next_line(fd);
            if(!line)
                break;
            if(line[ft_strlen(line) - 1] == '\n')
                line[ft_strlen(line) - 1] = '\0';
            add_to_history(line, history);
    }
    close(fd);
}

void custom_save_history(t_list **history)
{
    int fd;
    t_list *current;

    fd = open("./history", O_RDWR | O_CREAT, S_IRWXU);
    current = *history;
    while(current)
    {
        write(fd, current->content, ft_strlen(current->content));
        write(fd, "\n", 1);
        current = current->next;
    }
    ft_lstclear(history, free);
    rl_clear_history();
    close(fd);
}