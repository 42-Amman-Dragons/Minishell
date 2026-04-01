/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:16:50 by haya              #+#    #+#             */
/*   Updated: 2026/04/01 11:51:55 by haya             ###   ########.fr       */
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
