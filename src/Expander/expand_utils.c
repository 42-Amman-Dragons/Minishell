/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/26 10:32:27 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char(char *result, char c)
{
	char	tmp[2];
	char	*joined;

	tmp[0] = c;
	tmp[1] = '\0';
	if (!result)
		return (ft_strdup(tmp));
	joined = ft_strjoin(result, tmp);
	free(result);
	if (!joined)
		return (NULL);
	return (joined);
}

char	*append_str(char *result, char *s)
{
	char	*joined;

	if (!s)
		return (result);
	if (!result)
		return (s);
	joined = ft_strjoin(result, s);
	free(result);
	free(s);
	if (!joined)
		return (NULL);
	return (joined);
}

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
		free(name);
		return (ft_strdup("$"));
	}
	value = get_env_value(name, ctx->env);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
