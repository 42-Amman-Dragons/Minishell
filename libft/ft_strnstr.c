/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:23:35 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/17 12:44:12 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	len_l;

	i = 0;
	len_l = ft_strlen(little);
	if (len_l == 0)
		return ((char *)big);
	if (len == 0)
		return (NULL);
	while (big[i] && i < len)
	{
		if (len_l > len - i)
			return (NULL);
		if (ft_strncmp(&big[i], little, len_l) == 0)
		{
			return (&((char *)big)[i]);
		}
		i++;
	}
	return (NULL);
}
