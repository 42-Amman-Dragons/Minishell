/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:34:25 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/17 15:04:43 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if ((char)s[i] == (char)c)
		{
			return (((char *)s) + i);
		}
		i++;
	}
	if ((char)c == '\0')
	{
		return (((char *) s) + i);
	}
	return (NULL);
}
