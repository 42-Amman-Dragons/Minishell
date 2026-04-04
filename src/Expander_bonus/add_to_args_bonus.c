/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_args_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 12:36:37 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	normalize_ifs(char *s)
{
	while (*s)
	{
		if (*s == '\t' || *s == '\n')
			*s = ' ';
		s++;
	}
}

char	**add_to_args(char **args, int i, char *expanded, int *flags)
{
	char	*matched_paths;

	if (flags[1] && expanded && ft_strchr(expanded, '*'))
	{
		matched_paths = append_astersk(NULL, expanded);
		if (matched_paths)
		{
			free(expanded);
			expanded = matched_paths;
		}
	}
	if (expanded)
		restore_astersks(expanded);
	if (flags[1] && expanded)
	{
		if (!flags[0])
			normalize_ifs(expanded);
		if (contains(expanded, ' ') == 1)
			return (generate_expanded_list(args, i, expanded));
	}
	free(args[i]);
	args[i] = expanded;
	return (args);
}
