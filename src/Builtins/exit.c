/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/06 13:18:04 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_space_char(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_all_num(char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_space_char(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i] || !ft_isdigit(str[i]))
		return (0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && is_space_char(str[i]))
		i++;
	if (str[i] != '\0')
		return (0);
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

void	num_arg_req_err(char *arg, t_minishell *shell)
{
	char	*buff;

	buff = ft_strdup("");
	buff = safe_join(buff, "minishell: exit: ");
	buff = safe_join(buff, arg);
	buff = safe_join(buff, ": numeric argument required\n");
	ft_putstr_fd(buff, 2);
	if (buff)
		free(buff);
	buff = NULL;
	custom_save_history(shell);
	exit(cleanup_shell(shell, 2));
}

int	ft_exit(char **args, t_minishell *shell)
{
	if (calc_len_args(args) >= 2 && (!is_all_num(args[1])))
		num_arg_req_err(args[1], shell);
	if (calc_len_args(args) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (calc_len_args(args) == 2)
	{
		if (check_boundries(args[1]) == -1)
			num_arg_req_err(args[1], shell);
		custom_save_history(shell);
		exit(cleanup_shell(shell, ft_atoi(args[1])));
	}
	if (shell->is_interactive)
		ft_putstr_fd("exit\n", 2);
	custom_save_history(shell);
	if (shell->current_tree)
	{
		free_tree(shell->current_tree);
		shell->current_tree = NULL;
	}
	exit(cleanup_shell(shell, shell->exit_status));
	return (0);
}
