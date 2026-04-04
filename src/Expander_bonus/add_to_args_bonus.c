/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_args_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 20:31:39 by haya             ###   ########.fr       */
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

char	**handle_qouted_asterisk(char **args, int i, char *expanded)
{
	char	**matched_paths;
	char	**new_args;

	matched_paths = append_astersk(expanded);
	if (matched_paths)
	{
		new_args = generate_expanded_list_asterisk(args, i, matched_paths);
		if (!new_args)
		{
			free(expanded);
			return (free_args_expanded_asterisk(args, matched_paths));
		}
		free(expanded);
		return (new_args);
	}
	return (NULL);
}

char	**add_to_args(char **args, int i, char *expanded, int *flags)
{
	if (flags[1] && expanded && ft_strchr(expanded, '*'))
		return (handle_qouted_asterisk(args, i, expanded));
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
