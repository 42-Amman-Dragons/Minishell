/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 05:28:24 by mabuqare         ###   ########.fr       */
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

static char	**generate_expanded_list(char **args, int i, char *expanded)
{
	int		j;
	int		k;
	int		args_len;
	char	**new_args;

	args_len = calc_len_args(args);
	new_args = ft_calloc(args_len + cal_len(expanded, ' ') + 1, sizeof(char *));
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
			args[k] = NULL;
		}
		else
			new_args[j++] = args[k];
		k++;
	}
	free(args);
	free(expanded);
	return (new_args);
}

static void	normalize_ifs(char *s)
{
	while (*s)
	{
		if (*s == '\t' || *s == '\n')
			*s = ' ';
		s++;
	}
}

static char	**add_to_args(char **args, int i, char *expanded, int is_wc,
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

static char	*get_unquoted_var_val(char *word, int *i, char **env,
				int exit_status)
{
	int		j;
	char	*name;
	char	*val;

	j = *i + 1;
	if (!word[j])
		return (NULL);
	if (word[j] == '?')
	{
		*i = j;
		return (ft_itoa(exit_status));
	}
	if (word[j] == '$')
	{
		*i = j;
		return (NULL);
	}
	while (ft_isalnum(word[j]) || word[j] == '_')
		j++;
	if (j == *i + 1)
		return (NULL);
	name = ft_substr(word, *i + 1, j - *i - 1);
	if (!name)
		return (NULL);
	val = get_env_value(name, env);
	free(name);
	*i = j - 1;
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static int	unquoted_dollar_has_space(char *word, char **env, int exit_status)
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
			{
				free(val);
				return (1);
			}
			free(val);
		}
		i++;
	}
	return (0);
}

char	**expand_one_arg(char **args, int i, t_minishell *shell)
{
	char	*expanded;
	int		quoted;
	int		is_wc;

	quoted = word_has_quotes(args[i]);
	is_wc = unquoted_dollar_has_space(args[i], shell->env, shell->exit_status);
	expanded = expand_word(args[i], shell->env, shell->exit_status);
	if (!expanded)
		return (args);
	if (!quoted && expanded[0] == '\0')
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
	args = add_to_args(args, i, expanded, is_wc, quoted);
	return (args);
}
