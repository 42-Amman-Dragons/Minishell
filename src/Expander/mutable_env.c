/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutable_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/28 00:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_partial(char **arr, int count)
{
	while (count-- > 0)
		free(arr[count]);
	free(arr);
}

char	**init_mutable_env(char **env)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			free_partial(new_env, i);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
