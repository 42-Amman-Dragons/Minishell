/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:58:16 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/19 15:46:09 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_number(int n, int fd, char c)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		c = '-';
		write (fd, &c, 1);
		n = n * -1;
	}
	if (n >= 0 && n <= 9)
	{
		c = n + '0';
		write (fd, &c, 1);
		return ;
	}
	else
	{
		c = n % 10 + '0';
		ft_putnbr_fd(n / 10, fd);
		write(fd, &c, 1);
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	c = 0;
	print_number(n, fd, c);
}
