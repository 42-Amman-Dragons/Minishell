/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:57:32 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/03/19 05:19:31 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The add history function added a line to the history linked linst
and the readline history
returnd:
	-1: error
	0: fail
	1: success
*/
int	add_to_history(char *line, t_list **history)
{
	t_list	*node;
	char	*dup;

	if (ft_strlen(line) == 0)
		return (0);
	add_history(line);
	dup = ft_strdup(line);
	if (!dup)
		return (-1);
	node = ft_lstnew(dup);
	if (!node)
	{
		free(dup);
		return (-1);
	}
	ft_lstadd_back(history, node);
	return (1);
}

/*
The load history function adde history lines from a history file
and added them to the history linked linst and the readline history
*/
static char	*get_history_path(char **env)
{
	char	*home;
	char	*path;

	home = get_env_value("HOME", env);
	if (!home)
		home = getenv("HOME");
	if (!home)
		return (ft_strdup(".minishell_history"));
	path = ft_strjoin(home, "/.minishell_history");
	return (path);
}

static int	read_history_lines(int fd, t_list **history)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (add_to_history(line, history) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

/*
The custom save history function saves
the content of the history linked list to the history file
*/
void	custom_save_history(t_minishell *shell)
{
	int		fd;
	t_list	*current;
	char	*path;

	path = get_history_path(shell->env);
	if (!path)
		return ;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	free(path);
	if (fd == -1)
		return ;
	current = shell->history;
	while (current)
	{
		ft_putstr_fd(current->content, fd);
		ft_putchar_fd('\n', fd);
		current = current->next;
	}
	close(fd);
}

int	load_history(t_minishell *shell)
{
	int		fd;
	char	*path;

	path = get_history_path(shell->env);
	if (!path)
		return (-1);
	fd = open(path, O_RDWR | O_CREAT, S_IRWXU);
	free(path);
	if (fd == -1)
	{
		perror("minishell: history: Permission denied ");
		return (-1);
	}
	if (read_history_lines(fd, &shell->history) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
