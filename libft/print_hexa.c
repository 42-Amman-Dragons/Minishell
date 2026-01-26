/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:49:42 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/09/02 11:17:42 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	printnhex(unsigned int num, char type)
{
	char	*upper;
	char	*lower;

	upper = "ABCDEF";
	lower = "abcdef";
	if (num < 10)
		ft_putnbr_fd(num, 1);
	else
	{
		if (type == 'X')
			ft_putchar_fd(upper[num - 10], 1);
		else if (type == 'x')
			ft_putchar_fd(lower[num - 10], 1);
	}
}

static void	ft_puthex(unsigned int num, char type)
{
	if (num < 16)
	{
		printnhex(num, type);
		return ;
	}
	ft_puthex(num / 16, type);
	printnhex(num % 16, type);
}

int	print_hexa(unsigned int num, char type)
{
	int	size;

	size = 0;
	ft_puthex(num, type);
	if (num == 0)
		return (1);
	while (num)
	{
		num = num / 16;
		size ++;
	}
	return (size);
}
