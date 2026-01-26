/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:33:22 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/09/02 16:08:05 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	print_unsigned_number(unsigned int n, int fd, char c)
{
	if (n <= 9)
	{
		c = n + '0';
		write (fd, &c, 1);
		return ;
	}
	else
	{
		c = n % 10 + '0';
		print_unsigned_number(n / 10, fd, c);
		write(fd, &c, 1);
	}
}

int	print_unsigned(unsigned int num)
{
	char	c;
	int		size;

	c = '0';
	size = 0;
	print_unsigned_number(num, 1, c);
	if (num == 0)
		return (1);
	while (num)
	{
		num = num / 10;
		size++;
	}
	return (size);
}
