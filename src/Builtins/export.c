/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 04:19:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/01 12:40:39 by haya             ###   ########.fr       */
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
	char	*buff;

	buff = ft_strdup("");
	buff = safe_join(buff, "minishell: ");
	buff = safe_join(buff, "export: '");
	buff = safe_join(buff, id);
	buff = safe_join(buff, "': not a valid identifier\n");
	ft_putstr_fd(buff, 2);
	free(buff);
	buff = NULL;
	return (1);
}

static int	handle_not_eq(char *arg, char *dup, t_minishell *shell)
{
	if (!valid_identifier(arg))
		return (export_invalid(arg));
	if (!get_env_value(arg, shell->env))
	{
		dup = ft_strdup(arg);
		if (!dup || add_env(dup, shell) != 0)
		{
			free(dup);
			return (1);
		}
	}
	return (0);
}

static int	export_one(char *arg, t_minishell *shell)
{
	char	*eq;
	char	*name;
	int		ret;
	char	*dup;

	eq = ft_strchr(arg, '=');
	dup = "";
	if (!eq)
		return (handle_not_eq(arg, dup, shell));
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
