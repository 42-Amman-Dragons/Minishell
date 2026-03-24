/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutable_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 08:52:41 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mk_env_entry(char *name, char *value)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

int	init_mutable_env(char **env, t_minishell *shell)
{
	char	*entry;
	int		i;

	shell->env = malloc(sizeof(char *));
	if (!shell->env)
		return (1);
	shell->env[0] = NULL;
	i = 0;
	while (env[i])
	{
		entry = ft_strdup(env[i]);
		if (!entry || add_env(entry, shell) != 0)
		{
			free(entry);
			free_env(shell->env);
			shell->env = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
