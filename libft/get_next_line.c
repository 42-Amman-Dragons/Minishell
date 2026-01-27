/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 12:20:54 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/01/27 14:16:32 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static void	copy_to_line_and_reset(char *temp, char **line, char *buffer, int i)
{
	int	len;

	len = 0;
	if (*line)
		len = ft_strlen(*line);
	ft_memcpy(temp, *line, len);
	ft_memcpy(temp + len, buffer, i);
	ft_memcpy(buffer, buffer + i, BUFFER_SIZE - i);
	buffer[BUFFER_SIZE - i] = '\0';
}

static char	*add_to_line(char *buffer, char **line)
{
	int		i;
	char	*temp;
	int		len;

	i = 0;
	len = 0;
	if (*line)
		len = ft_strlen(*line);
	while (buffer[i])
	{
		if (buffer[i++] == '\n')
			break ;
	}
	if (i == 0)
		return (NULL);
	temp = malloc((len + i + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	ft_memset(temp, '\0', len + i + 1);
	copy_to_line_and_reset(temp, line, buffer, i);
	free(*line);
	*line = temp;
	return (*line);
}

static void	set_buffer(char **buffer)
{
	if (!(*buffer))
	{
		(*buffer) = malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!(*buffer))
			return ;
		ft_memset((*buffer), '\0', BUFFER_SIZE + 1);
	}
}

static ssize_t	read_line(int fd, char *buffer, char **line)
{
	ssize_t	bytes_read;

	bytes_read = 0;
	if (buffer[0])
	{
		add_to_line(buffer, line);
		if (!(*line) || ft_strchr((*line), '\n'))
			return (bytes_read);
	}
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (bytes_read);
		buffer[bytes_read] = '\0';
		add_to_line(buffer, line);
		if (!(*line) || ft_strchr((*line), '\n'))
			return (bytes_read);
	}
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	set_buffer(&buffer);
	if (!buffer)
		return (NULL);
	bytes_read = read_line(fd, buffer, &line);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (return_line(&line, &buffer, bytes_read));
}
