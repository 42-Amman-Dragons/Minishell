/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:12:26 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/17 14:54:53 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calstrlen(int n)
{
	int	i;

	i = 0;
	if (n == -2147483648)
	{
		return (11);
	}
	if (n < 0)
	{
		i++;
		n = n * -1;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static void	addn(char **str, long n, int i)
{
	if (n < 0)
	{
		(*str)[0] = '-';
		n = n * -1;
	}
	if (n >= 0 && n <= 9)
	{
		(*str)[i - 1] = n + '0';
		return ;
	}
	else
	{
		addn(str, n / 10, i - 1);
		(*str)[i - 1] = n % 10 + '0';
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;

	len = calstrlen(n);
	str = malloc((len * sizeof(char)) + 1);
	if (!str)
		return (NULL);
	addn(&str, n, len);
	str[len] = '\0';
	return (str);
}
