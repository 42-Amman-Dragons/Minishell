/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/05 02:45:01 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	word_has_unquoted_asterisk(char *word)
{
	int	sq;
	int	dq;

	sq = 0;
	dq = 0;
	while (*word)
	{
		if (*word == '\'' && !dq)
			sq = !sq;
		else if (*word == '"' && !sq)
			dq = !dq;
		else if (*word == '*' && !sq && !dq)
			return (1);
		word++;
	}
	return (0);
}

// flags[0] quoted
// flags[1] unquoted $ has space or wildcard
char	**expand_one_arg(char **args, int i, t_minishell *shell)
{
	char	*expanded;
	int		flags[2];

	flags[0] = word_has_quotes(args[i]);
	flags[1] = unquoted_dollar_has_space(args[i], shell->env,
			shell->exit_status) || word_has_unquoted_asterisk(args[i]);
	expanded = expand_word(args[i], shell->env, shell->exit_status);
	if (!expanded)
		return (args);
	if (!flags[1] && !flags[0] && ft_strchr(expanded, '*'))
		flags[1] = 1;
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
