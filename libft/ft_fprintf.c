/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 10:29:01 by haya              #+#    #+#             */
/*   Updated: 2026/03/30 10:55:23 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_fprintf.h"

static int	ft_putchar_fd_mod(char c, int fd)
{
	ft_putchar_fd(c, fd);
	return (1);
}

static int	helper_print(va_list list, char type, int*j, int fd)
{
	*j = (*j) + 2;
	if (type == 'c')
		return (ft_putchar_fd_mod(va_arg(list, int), fd));
	else if (type == 's')
		return (print_string(va_arg(list, char *), fd));
	else
		return (ft_putchar_fd_mod('%', fd) + ft_putchar_fd_mod(type, fd));
    }

static int	write_segment(const char *format, int i, int *j, int fd)
{
	while (format[*j] && format[*j] != '%')
		(*j)++;
	if (*j != i)
		write(fd, format + i, *j - i);
	return (*j - i);
}

int	ft_fprintf(int fd, const char *format, ...)
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
			size += write_segment(format, i, &j , fd);
		else
		{
			if (!format[i + 1])
				return (-1);
			size += helper_print(list, format[i + 1], &j, fd);
		}
		i = j;
	}
	va_end(list);
	return (size);
}
