/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_nonint.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 00:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_heredoc_file(int fd, char *lim, t_redir_data *rd,
		t_minishell *shell)
{
	char	*line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strncmp(line, lim, ft_strlen(lim)) == 0
			&& ft_strlen(line) == ft_strlen(lim))
		{
			free(line);
			return ;
		}
		push_heredoc_line(fd, line, rd, shell);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	print_eof_warning(lim);
}

int	read_heredoc_nointeractive(t_redir_data *rd, t_minishell *shell, char *tmp)
{
	int	fd;

	fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	fill_heredoc_file(fd, rd->filename, rd, shell);
	close(fd);
	rd->heredoc_fd = open(tmp, O_RDONLY);
	unlink(tmp);
	if (rd->heredoc_fd < 0)
		return (-1);
	return (0);
}
