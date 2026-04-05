/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutable_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/05 18:16:06 by mabuqare         ###   ########.fr       */
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

static void	shlvl_error_message(int shell_lvl)
{
	char	*buff;

	buff = ft_strdup("minishell: warning: shell level (");
	buff = safe_join(buff, ft_itoa(shell_lvl + 1));
	buff = safe_join(buff, ") too high, resetting to 1\n");
	ft_putstr_fd(buff, 2);
	free(buff);
	buff = NULL;
}

static void	increment_shlvl(t_minishell *shell)
{
	char	*shlvl_str;
	long	shlvl;
	char	*new_val;

	shlvl_str = get_env_value("SHLVL", shell->env);
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	else
		shlvl = 0;
	if (shlvl >= 999)
	{
		shlvl_error_message(shlvl);
		shlvl = 1;
	}
	else if (shlvl < 0)
		shlvl = 0;
	else
		shlvl = shlvl + 1;
	new_val = ft_itoa(shlvl);
	if (new_val)
	{
		set_env_value("SHLVL", new_val, shell);
		free(new_val);
	}
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
	increment_shlvl(shell);
	return (0);
}
