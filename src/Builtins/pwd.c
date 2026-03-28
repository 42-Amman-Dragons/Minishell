/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 02:57:13 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/28 21:19:27 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_pwd_line(char *pwd, int should_free)
{
	if (!pwd)
		return (1);
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	if (should_free)
		free(pwd);
	return (0);
}

int	ft_pwd(t_minishell *shell)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
		return (print_pwd_line(pwd, 1));
	return (print_pwd_line(get_env_value("PWD", shell->env), 0));
}
