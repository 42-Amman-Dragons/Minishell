/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:12:34 by haya              #+#    #+#             */
/*   Updated: 2026/04/05 02:45:01 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	**free_args_expanded_return(char **args, char **expanded,
		char **ret)
{
	if (!ret)
	{
		free_args(args);
		free(*expanded);
		*expanded = NULL;
		return (NULL);
	}
	free(args);
	free(*expanded);
	*expanded = NULL;
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
		return (free_args_expanded_return(args, &expanded, NULL));
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
	return (free_args_expanded_return(args, &expanded, new_args));
}

char	**add_to_args(char **args, int i, char *expanded, int *flags)
{
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

// flags[0] quoted
// flags[1] unquoted $ has space
char	**expand_one_arg(char **args, int i, t_minishell *shell)
{
	char	*expanded;
	int		flags[2];

	flags[0] = word_has_quotes(args[i]);
	flags[1] = unquoted_dollar_has_space(args[i], shell->env,
			shell->exit_status);
	expanded = expand_word(args[i], shell->env, shell->exit_status);
	if (!expanded)
		return (args);
	if (!flags[0] && expanded[0] == '\0')
	{
		free(expanded);
		free(args[i]);
		while (args[i])
		{
			args[i] = args[i + 1];
			i++;
		}
		return (args);
	}
	args = add_to_args(args, i, expanded, flags);
	return (args);
}
