/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_matching_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare <mabuqare@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 10:30:16 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:40:59 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	match_special(char *pattern, char *file_name)
{
	if (*pattern == '\x01')
	{
		if (*file_name == '*')
			return (is_matching(pattern + 1, file_name + 1));
		return (0);
	}
	return (-1);
}

static int	match_wildcard(char *pattern, char *file_name)
{
	while (*pattern == '*')
		pattern++;
	if (!*pattern)
		return (1);
	while (*file_name)
	{
		if (is_matching(pattern, file_name))
			return (1);
		file_name++;
	}
	return (0);
}

static int	match_literal(char *pattern, char *file_name)
{
	if (*pattern == *file_name)
		return (is_matching(pattern + 1, file_name + 1));
	return (0);
}

int	is_matching(char *pattern, char *file_name)
{
	int	special;

	special = match_special(pattern, file_name);
	if (special != -1)
		return (special);
	if (*pattern == '*')
		return (match_wildcard(pattern, file_name));
	if (!*pattern && !*file_name)
		return (1);
	if (!*pattern || !*file_name)
		return (0);
	return (match_literal(pattern, file_name));
}
