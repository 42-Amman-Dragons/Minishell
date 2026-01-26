/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 09:51:44 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/09/02 16:08:07 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putchar_fd_mod(char c)
{
	ft_putchar_fd(c, 1);
	return (1);
}

static int	helper_print(va_list list, char type, int*j)
{
	*j = (*j) + 2;
	if (type == 'c')
		return (ft_putchar_fd_mod(va_arg(list, int )));
	else if (type == 's')
		return (print_string(va_arg(list, char *)));
	else if (type == 'p')
		return (print_pointer(va_arg(list, void *)));
	else if (type == 'd' || type == 'i')
		return (print_number(va_arg(list, int)));
	else if (type == 'u')
		return (print_unsigned(va_arg(list, unsigned int)));
	else if (type == 'x' || type == 'X')
		return (print_hexa(va_arg(list, unsigned int), type));
	else if (type == '%')
		return (ft_putchar_fd_mod('%'));
	else
		return (ft_putchar_fd_mod('%') + ft_putchar_fd_mod(type));
}

static int	write_segment(const char *format, int i, int *j)
{
	while (format[*j] && format[*j] != '%')
		(*j)++;
	if (*j != i)
		write(1, format + i, *j - i);
	return (*j - i);
}

int	ft_printf(const char *format, ...)
{
	va_list	list;
	int		i;
	int		j;
	int		size;

	i = 0;
	j = 0;
	size = 0;
	if (!format)
		return (-1);
	va_start(list, format);
	while (format[i])
	{
		if (format[i] != '%')
			size += write_segment(format, i, &j);
		else
		{
			if (!format[i + 1])
				return (-1);
			size += helper_print(list, format[i + 1], &j);
		}
		i = j;
	}
	va_end(list);
	return (size);
}
