/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/23 21:20:46 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	word_has_quotes(char *word)
{
	while (*word)
	{
		if (*word == '\'' || *word == '"')
			return (1);
		word++;
	}
	return (0);
}

void	strip_empty_args(t_tree *node, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		if (node->data.cmd.args[i] != NULL)
			node->data.cmd.args[j++] = node->data.cmd.args[i];
		i++;
	}
	node->data.cmd.args[j] = NULL;
	if (j == 0)
	{
		free(node->data.cmd.args);
		node->data.cmd.args = NULL;
	}
}

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

char	**generate_expanded_list(char **args, int i, char *expanded)
{
	int		j;
	int		k;
	int		args_len;
	int		expanded_len;
	char	**new_args;

	args_len = calc_len_args(args);
	expanded_len = cal_len(expanded, ' ');
	new_args = ft_calloc(args_len + expanded_len + 1, sizeof(char *));
	if (!new_args)
	{
		free_args(args);
		free(expanded);
		return (NULL);
	}
	j = 0;
	k = 0;
	while (k < args_len)
	{
		if (k == i)
		{
			add_splitted(new_args, &j, expanded);
			free(args[k]);
		}
		else
			copy_arg(new_args, args, &j, k);
		k++;
	}
	free(expanded);
	free(args);
	return (new_args);
}

char	**add_to_args(char **args, int i, char *expanded, int is_wc)
{
	if (is_wc && expanded && contains(expanded, ' ') == 1)
		args = generate_expanded_list(args, i, expanded);
	else
	{
		free(args[i]);
		args[i] = expanded;
	}
	return (args);
}

char	**expand_one_arg(char **args, int i, t_minishell *shell)
{
	char	*expanded;
	int		quoted;
	int		is_wc;

	quoted = word_has_quotes(args[i]);
	is_wc = (!quoted && contains(args[i], '*'));
	expanded = expand_word(args[i], shell->env, shell->exit_status);
	if (!expanded)
		return (args);
	if (!quoted && expanded[0] == '\0')
	{
		if (is_wc)
		{
			free(expanded);
			expanded = NULL;
			return (args);
		}
		free(expanded);
		expanded = NULL;
	}
	args = add_to_args(args, i, expanded, is_wc);
	return (args);
}
