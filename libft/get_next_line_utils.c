/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 12:21:28 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/03/24 08:43:11 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*return_line(char **line, char **buffer, int bytes_read)
{
	if (ft_strchr(*line, '\n'))
		return (*line);
	if (bytes_read < 0)
	{
		if (*line)
		{
			free(*line);
			*line = NULL;
		}
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	if (bytes_read == 0)
	{
		free(*buffer);
		*buffer = NULL;
		return (*line);
	}
	return (*line);
}
