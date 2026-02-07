/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:57:32 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/02/06 23:07:50 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The add history function added a line to the history linked linst and the readline history
returnd:
	-1: error
	0: fail
	1: success
*/
int	add_to_history(char *line, t_list **history)
{
	t_list	*node;

	if (ft_strlen(line) == 0)
		return (0);
	add_history(line);
	node = ft_lstnew(line);
	if (!node)
		return (-1);
	ft_lstadd_back(history, node);
	return (1);
}

/*
The load history function adde history lines from a history file and added them to the history linked linst and the readline history
*/

void	load_history(t_list **history)
{
	int		fd;
	char	*line;

	fd = open("./.history", O_RDWR | O_CREAT, S_IRWXU);
	if (access("./.history", F_OK) != 0)
		ft_putstr_fd("cannot find history file\n", 2);
	if (access("./.history", R_OK) != 0)
		ft_putstr_fd("read permission denied in the history\n", 2);
	if (access("./.history", W_OK) != 0)
		ft_putstr_fd("write permission denied in the history\n", 2);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		add_to_history(line, history);
	}
	close(fd);
}

/*
The custom save history function saves the content of the history linked list to the history file
*/
void	custom_save_history(t_list **history)
{
	int fd;
	t_list *current;

	fd = open("./.history", O_RDWR | O_CREAT, S_IRWXU);
	current = *history;
	while (current)
	{
		write(fd, current->content, ft_strlen(current->content));
		write(fd, "\n", 1);
		current = current->next;
	}
	close(fd);
}