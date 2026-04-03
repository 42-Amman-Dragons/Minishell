/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_expanded_list_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 13:28:15 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/04 00:12:12 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	add_splitted(char **new_args, int *j, char *expanded)
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

static void	copy_arg(char **new_args, char **args, int *j, int k)
{
	new_args[*j] = args[k];
	(*j)++;
}

static void	*free_args_expanded(char **args, char *expanded)
{
	free_args(args);
	free(expanded);
	return (NULL);
}

static void	fill_expanded_args(char **new_args, char **args, int i,
		char *expanded)
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
			add_splitted(new_args, &j, expanded);
			free(args[k]);
			args[k] = NULL;
		}
		else
			copy_arg(new_args, args, &j, k);
		k++;
	}
}

char	**generate_expanded_list(char **args, int i, char *expanded)
{
	int		args_len;
	char	**new_args;

	args_len = calc_len_args(args);
	new_args = ft_calloc(args_len + cal_len(expanded, ' ') + 1, sizeof(char *));
	if (!new_args)
		return (free_args_expanded(args, expanded));
	fill_expanded_args(new_args, args, i, expanded);
	free(args);
	free(expanded);
	return (new_args);
}
