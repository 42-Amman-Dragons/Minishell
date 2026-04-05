/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_shared.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/05 02:39:41 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	free_val_return(char *val)
{
	free(val);
	return (1);
}

int	unquoted_dollar_has_space(char *word, char **env, int exit_status)
{
	int		sq;
	int		dq;
	int		i;
	char	*val;

	sq = 0;
	dq = 0;
	i = 0;
	while (word[i])
	{
		if (word[i] == '\'' && !dq)
			sq = !sq;
		else if (word[i] == '"' && !sq)
			dq = !dq;
		else if (word[i] == '$' && !sq && !dq)
		{
			val = get_unquoted_var_val(word, &i, env, exit_status);
			if (val && (contains(val, ' ') || contains(val, '\t')
					|| contains(val, '\n')))
				return (free_val_return(val));
			free(val);
		}
		i++;
	}
	return (0);
}
