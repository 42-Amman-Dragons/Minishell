/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_expanded_list_asterisk_bonus.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 19:37:51 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:34:44 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	add_splitted_asterisk(char **new_args, int *j, char **expanded)
{
	int	i;

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

void	*free_args_expanded_asterisk(char **args, char **expanded)
{
	free_args(args);
	free_args(expanded);
	return (NULL);
}

char	**generate_expanded_list_asterisk(char **args, int i, char **expanded)
{
	int		args_len;
	int		expanded_len;
	char	**new_args;

	args_len = calc_len_args(args);
	expanded_len = calc_len_args(expanded);
	new_args = ft_calloc(args_len + expanded_len + 1, sizeof(char *));
	if (!new_args)
		return (free_args_expanded_asterisk(args, expanded));
	fill_expanded_args_asterisk(new_args, args, i, expanded);
	free(args);
	return (new_args);
}
