/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_args_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 14:40:26 by haya             ###   ########.fr       */
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

static void	add_splitted_asterisk(char **new_args, int *j, char **expanded)
{
	int		i;

	i = 0;
	if (!expanded)
		return ;
	while (expanded[i])
	{
		new_args[*j] = ft_strdup(expanded[i]);
		free(expanded[i]);
		expanded[i] = NULL;
		(*j)++;
		i++;
	}
	free(expanded);
}

static void	fill_expanded_args_asterisk(char **new_args, char **args, int i,
		char **expanded)
{
	int	j;
	int	k;
	int	args_len;

	args_len = calc_len_args(args);
	j = 0;
	k = 0;
	while (k < args_len)
	{
		if (k == i)
		{
			add_splitted_asterisk(new_args, &j, expanded);
			free(args[k]);
			args[k] = NULL;
		}
		else
		{
				new_args[j] = args[k];
				j++;
		}
		k++;
	}
}


static void	*free_args_expanded_asterisk(char **args, char **expanded)
{
	free_args(args);
	free_args(expanded);
	return (NULL);
}

char	**generate_expanded_list_asterisk(char **args, int i, char **expanded)
{
	int		args_len;
	int     expanded_len;
	char	**new_args;

	args_len = calc_len_args(args);
	expanded_len = calc_len_args(expanded);
	new_args = ft_calloc(args_len + expanded_len + 1, sizeof(char *));
	if (!new_args)
		return (free_args_expanded_asterisk(args, expanded));
	fill_expanded_args_asterisk(new_args, args, i, expanded);
	return (new_args);
}


char	**add_to_args(char **args, int i, char *expanded, int *flags)
{
	char	**matched_paths;
	char	**new_args;

	if (flags[1] && expanded && ft_strchr(expanded, '*'))
	{
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
