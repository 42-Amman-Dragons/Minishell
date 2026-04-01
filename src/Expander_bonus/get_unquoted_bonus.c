/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_unquoted_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 13:17:16 by haya              #+#    #+#             */
/*   Updated: 2026/04/01 10:46:54 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
