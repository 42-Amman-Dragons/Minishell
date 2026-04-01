/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_args_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by haya              #+#    #+#             */
/*   Updated: 2026/04/01 11:05:54 by haya             ###   ########.fr       */
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
	char	*globbed;

	if (flags[1] && expanded && ft_strchr(expanded, '*'))
	{
		globbed = append_astersk(NULL, expanded);
		if (globbed)
		{
			free(expanded);
			expanded = globbed;
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
