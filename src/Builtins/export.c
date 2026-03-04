/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 04:19:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/03 22:30:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_identifier(char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_invalid(char *id)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(id, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static int	export_one(char *arg, t_minishell *shell)
{
	char	*eq;
	char	*name;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		if (!valid_identifier(arg))
			return (export_invalid(arg));
		if (!get_env_value(arg, shell->env))
			add_env(ft_strdup(arg), shell);
		return (0);
	}
	name = ft_substr(arg, 0, eq - arg);
	if (!name)
		return (1);
	if (!valid_identifier(name))
	{
		ret = export_invalid(name);
		free(name);
		return (ret);
	}
	ret = set_env_value(name, eq + 1, shell);
	free(name);
	return (ret);
}

int	ft_export(char **args, t_minishell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
		return (print_sorted_env(shell));
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (export_one(args[i], shell) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
