/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:28:35 by haya              #+#    #+#             */
/*   Updated: 2026/03/31 10:30:12 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_spaces_sign(const char *nptr, int *i, int *sign)
{
	while (nptr[*i] == ' ' || (nptr[*i] >= 9 && nptr[*i] <= 13))
		(*i)++;
	if (nptr[*i] == '+' || nptr[*i] == '-')
	{
		if (nptr[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

int	check_boundries(const char *nptr)
{
	int					i;
	unsigned long long	num;
	int					sign;

	i = 0;
	num = 0;
	sign = 1;
	skip_spaces_sign(nptr, &i, &sign);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (num > (unsigned long long)__LONG_LONG_MAX__ / 10 + 1)
			return (-1);
		num = 10 * num + (nptr[i] - '0');
		i++;
	}
	if (sign == 1 && num > (unsigned long long)__LONG_LONG_MAX__)
		return (-1);
	if (sign == -1 && num > (unsigned long long)__LONG_LONG_MAX__ + 1)
		return (-1);
	return (0);
}
