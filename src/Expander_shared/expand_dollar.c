/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 02:39:59 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/07 17:28:55 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup(""));
	return (ft_substr(str, start, *i - start));
}

char	*expand_dollar(char *word, t_expand *ctx)
{
	char	*name;
	char	*value;

	ctx->i++;
	if (word[ctx->i] == '?')
	{
		ctx->i++;
		return (ft_itoa(ctx->exit_status));
	}
	name = extract_var_name(word, &ctx->i);
	if (!name || name[0] == '\0')
	{
		if (name)
			free(name);
		return (ft_strdup("$"));
	}
	value = get_env_value(name, ctx->env);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*return_val(char *name, char **env, int *i, int j)
{
	char	*val;

	val = get_env_value(name, env);
	free(name);
	*i = j - 1;
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

char	*get_unquoted_var_val(char *word, int *i, char **env, int exit_status)
{
	int		j;
	char	*name;

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
	return (return_val(name, env, i, j));
}
