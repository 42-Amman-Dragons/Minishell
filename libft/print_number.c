/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:12:48 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/09/02 11:18:07 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_number(int num)
{
	int	size;

	size = 0;
	ft_putnbr_fd(num, 1);
	if (num == 0)
		return (1);
	if (num < 0)
		size++;
	while (num)
	{
		size++;
		num /= 10;
	}
	return (size);
}
