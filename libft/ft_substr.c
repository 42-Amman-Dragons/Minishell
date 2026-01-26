/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:45:30 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/19 15:44:42 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sup;
	size_t	i;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start > slen)
		return (ft_calloc(1, sizeof(char)));
	if (start + len > slen)
		sup = malloc(slen - start + 1);
	else
		sup = malloc(len + 1);
	i = 0;
	if (!sup)
		return (NULL);
	while (i < len && s[start + i])
	{
		sup[i] = s[start + i];
		i++;
	}
	sup[i] = '\0';
	return (sup);
}
