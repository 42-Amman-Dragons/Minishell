/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by haya              #+#    #+#             */
/*   Updated: 2026/03/30 12:16:32 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	normalize_ifs(char *s)
{
	while (*s)
	{
		if (*s == '\t' || *s == '\n')
			*s = ' ';
		s++;
	}
}

static void	add_splitted(char **new_args, int *j, char *expanded)
{
	int		i;
	char	**splitted;

	i = 0;
	splitted = ft_split(expanded, ' ');
	if (!splitted)
		return ;
	while (splitted[i])
	{
		new_args[*j] = splitted[i];
		(*j)++;
		i++;
	}
	free(splitted);
}

static char	**free_args_expanded_return(char **args, char *expanded, char **ret)
{
		free_args(args);
		free(expanded);
		return (ret);
}

static char	**generate_expanded_list(char **args, int i, char *expanded)
{
	int		j;
	int		k;
	int		args_len;
	char	**new_args;

	args_len = calc_len_args(args);
	new_args = ft_calloc(args_len + cal_len(expanded, ' ') + 1, sizeof(char *));
	if (!new_args)
		return free_args_expanded_return(args, expanded, NULL);
	j = 0;
	k = 0;
	while (k < args_len)
	{
		if (k == i)
		{
			add_splitted(new_args, &j, expanded);
			free(args[k]);
			args[k] = NULL;
		}
		else
			new_args[j++] = args[k];
		k++;
	}
	return (free_args_expanded_return(args, expanded, new_args));
}

char	**add_to_args(char **args, int i, char *expanded, int is_wc,
				int quoted)
{
	if (is_wc && expanded)
	{
		if (!quoted)
			normalize_ifs(expanded);
		if (contains(expanded, ' ') == 1)
			return (generate_expanded_list(args, i, expanded));
	}
	free(args[i]);
	args[i] = expanded;
	return (args);
}