/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 00:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/22 00:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*safe_join(char *str1, char *str2)
{
	char	*result;

	result = ft_strjoin(str1, str2);
	free(str1);
	return (result);
}
