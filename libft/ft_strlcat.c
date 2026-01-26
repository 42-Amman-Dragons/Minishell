/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:20:53 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/17 16:54:29 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dst[i])
		i++;
	if (size - 1 < i)
		return (ft_strlen(src) + size);
	while ((i < size - 1) && src[j] && size > 0)
	{
		dst[i] = src[j];
		j++;
		i++;
	}
	if (size > 0)
		dst[i] = '\0';
	if (size == 0)
		i = 0;
	while (src[j])
	{
		j++;
		i++;
	}
	return (i);
}
