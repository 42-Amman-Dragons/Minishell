/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_args_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare <mabuqare@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:40:26 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	calc_arr_len(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}

char	**handle_qouted_asterisk(char **args, int i, char *expanded)
{
	char	**matched_paths;
	char	**new_args;

	matched_paths = append_astersk(expanded);
	if (!matched_paths)
		return (args);
	new_args = generate_expanded_list_asterisk(args, i, matched_paths);
	if (!new_args)
	{
		free(expanded);
		return (free_args_expanded_asterisk(args, matched_paths));
	}
	free(expanded);
	return (new_args);
}

char	**add_to_args(char **args, int i, char *expanded, int *flags)
{
	char	**result;

	if (flags[1] && expanded && ft_strchr(expanded, '*'))
	{
		result = handle_qouted_asterisk(args, i, expanded);
		if (result != args)
			return (result);
	}
	if (expanded)
		restore_astersks(expanded);
	if (flags[1] && expanded)
	{
		if (!flags[0])
			normalize_spaces(expanded);
		if (contains(expanded, ' ') == 1)
			return (generate_expanded_list(args, i, expanded));
	}
	free(args[i]);
	args[i] = expanded;
	return (args);
}
