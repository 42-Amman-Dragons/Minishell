/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 22:30:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/03 22:30:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static void	sort_idx(int *idx, char **env, int count)
{
	int	i;
	int	j;
	int	tmp;

	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
		{
			if (ft_strncmp(env[idx[i]], env[idx[j]],
					ft_strlen(env[idx[i]]) + 1) > 0)
			{
				tmp = idx[i];
				idx[i] = idx[j];
				idx[j] = tmp;
			}
		}
	}
}

static int	*get_sorted_idx(char **env, int count)
{
	int	*idx;
	int	i;

	idx = malloc(sizeof(int) * count);
	if (!idx)
		return (NULL);
	i = -1;
	while (++i < count)
		idx[i] = i;
	sort_idx(idx, env, count);
	return (idx);
}

static void	print_export_entry(char *entry)
{
	int	i;

	i = 0;
	ft_putstr_fd("declare -x ", 1);
	while (entry[i] && entry[i] != '=')
	{
		ft_putchar_fd(entry[i], 1);
		i++;
	}
	if (!entry[i])
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	ft_putstr_fd("=\"", 1);
	i++;
	ft_putstr_fd(entry + i, 1);
	ft_putstr_fd("\"\n", 1);
}

int	print_sorted_env(t_minishell *shell)
{
	int	*idx;
	int	count;
	int	i;

	count = env_count(shell->env);
	idx = get_sorted_idx(shell->env, count);
	if (!idx)
		return (1);
	i = 0;
	while (i < count)
	{
		print_export_entry(shell->env[idx[i]]);
		i++;
	}
	free(idx);
	return (0);
}
