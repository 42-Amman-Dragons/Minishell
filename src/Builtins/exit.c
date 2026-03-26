/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/26 12:34:16 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_all_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	calc_len_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	check_boundries(const char *nptr)
{
	int	i;
	long long	num;
	int	sign;

	i = 0;
	num = 0;
	sign = 1;
	while (isspace(nptr[i]) == 1)
	{
		i = i + 1;
	}
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
		{
			sign = sign * -1;
		}
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = 10 * num + (nptr[i] - '0');
		if (sign == -1 && num > (-1*__LONG_MAX__) - 1)
			return (-1);
		else if(sign && num > __LONG_MAX__)
			return (-1);
		i++;
	}
	return (0);
}


int	ft_exit(char **args, t_minishell *shell)
{
	if (calc_len_args(args) >= 2 
	&& 
	(!is_all_num(args[1])))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		custom_save_history(shell);
		exit(cleanup_shell(shell, 2));
	}
	if (calc_len_args(args) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (calc_len_args(args) == 2)
	{
		custom_save_history(shell);
		exit(cleanup_shell(shell, ft_atoi(args[1])));
	}
	ft_putstr_fd("exit\n", 1);
	custom_save_history(shell);
	exit(cleanup_shell(shell, shell->exit_status));
	return (0);
}
