/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:10:44 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/17 16:15:21 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	if ((char)c == '\0')
	{
		return ((char *)s + len);
	}
	while (len > 0)
	{
		if (*(s + len - 1) == (char )c)
		{
			return ((char *)s + len - 1);
		}
		len--;
	}
	return (NULL);
}
