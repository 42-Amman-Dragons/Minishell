/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 12:07:21 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/10/13 10:47:34 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include "libft.h"

int	ft_printf(const char *format, ...);
int	print_string(char *str);
int	print_pointer(void *pointer);
int	print_number(int num);
int	print_unsigned(unsigned int num);
int	print_hexa(unsigned int num, char type);

#endif
