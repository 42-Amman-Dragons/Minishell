/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:26:52 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/19 15:31:34 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	cal_len(char const *s, char c)
{
	int	j;
	int	k;
	int	size;

	j = 0;
	k = 0;
	size = 0;
	while (s[j])
	{
		if (s[j] != c)
		{
			while (s[k] && s[k] != c)
				k++;
			size++;
			j = k;
		}
		else
		{
			j++;
			k++;
		}
	}
	return (size);
}

static void	increase(int *j, int *k)
{
	(*j)++;
	(*k)++;
}

static char	**initalize(int *j, int *k, char const *s, char c)
{
	char	**out;

	if (!s)
		return (NULL);
	*j = 0;
	*k = 0;
	out = malloc(sizeof(char *) * (cal_len(s, c) + 1));
	if (!out)
		return (NULL);
	return (out);
}

static int	check_and_free(char ***out, int i)
{
	int	l_index;

	l_index = 0;
	if (!(*out)[i])
	{
		while ((*out)[l_index])
		{
			free((*out)[l_index]);
			l_index++;
		}
		free(*out);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**out;
	int		i;
	int		j;
	int		k;

	i = 0;
	out = initalize(&j, &k, s, c);
	if (!out)
		return (NULL);
	while (s[j])
	{
		if (s[j] != c)
		{
			while (s[k] && s[k] != c)
				k++;
			out[i++] = ft_substr(s, j, k - j);
			if (!check_and_free(&out, i - 1))
				return (NULL);
			j = k;
		}
		else
			increase(&j, &k);
	}
	out[i] = NULL;
	return (out);
}
