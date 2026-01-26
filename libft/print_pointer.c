/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:22:16 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/09/02 11:18:10 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	printnhexa(uintptr_t num)
{
	if (num < 10)
		ft_putnbr_fd(num, 1);
	else
	{
		if (num == 10)
			ft_putchar_fd('a', 1);
		if (num == 11)
			ft_putchar_fd('b', 1);
		if (num == 12)
			ft_putchar_fd('c', 1);
		if (num == 13)
			ft_putchar_fd('d', 1);
		if (num == 14)
			ft_putchar_fd('e', 1);
		if (num == 15)
			ft_putchar_fd('f', 1);
	}
}

static void	ft_puthex(uintptr_t num)
{
	if (num < 16)
	{
		printnhexa(num);
		return ;
	}
	ft_puthex(num / 16);
	printnhexa(num % 16);
}

int	print_pointer(void *pointer)
{
	int			size;
	uintptr_t	value;

	size = 0;
	if (!pointer)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	value = (uintptr_t)pointer;
	ft_putstr_fd("0x", 1);
	ft_puthex(value);
	if (value == 0)
		return (1);
	while (value)
	{
		value = value / 16;
		size++;
	}
	return (size + 2);
}
