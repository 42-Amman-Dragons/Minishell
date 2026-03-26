/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_expanded_list.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 13:28:15 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/03/26 13:02:41 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**generate_expanded_list(char **args, int i, char *expanded)
{
	int		j;
	int		k;
	int		args_len;
	char	**new_args;

	args_len = calc_len_args(args);
	new_args = ft_calloc(args_len + cal_len(expanded, ' ') + 1, sizeof(char *));
	if (!new_args)
		return free_args_expanded(args, expanded);
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
	free_args_expanded(args, expanded);
	return (new_args);
}
