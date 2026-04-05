/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/05 02:44:20 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*safe_join(char *str1, char *str2)
{
	char	*result;

	if (!str1)
		return (NULL);
	result = ft_strjoin(str1, str2);
	free(str1);
	return (result);
}

char	*append_char(char *result, char c)
{
	char	tmp[2];
	char	*joined;

	tmp[0] = c;
	tmp[1] = '\0';
	if (!result)
		return (ft_strdup(tmp));
	joined = ft_strjoin(result, tmp);
	free(result);
	if (!joined)
		return (NULL);
	return (joined);
}

char	*append_str(char *result, char *s)
{
	char	*joined;

	if (!s)
		return (result);
	if (!result)
		return (s);
	joined = ft_strjoin(result, s);
	free(result);
	free(s);
	if (!joined)
		return (NULL);
	return (joined);
}

void	normalize_spaces(char *s)
{
	while (*s)
	{
		if (*s == '\t' || *s == '\n')
			*s = ' ';
		s++;
	}
}
