/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 13:27:59 by hal-lawa         ###   ########.fr       */
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
