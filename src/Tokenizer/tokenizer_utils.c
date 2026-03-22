/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:31:51 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/22 18:12:20 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

void	skip_whitespaces(char *ptr, int *i)
{
	while (ptr[*i] && is_whitespace(ptr[*i]))
		(*i)++;
}

t_tokenType	identify_token(char *s)
{
	if (ft_strncmp(">>", s, 2) == 0 || ft_strncmp("<<", s, 2) == 0)
		return (REDIRECT);
	if (ft_strncmp("&&", s, 2) == 0)
		return (AND);
	if (ft_strncmp("||", s, 2) == 0)
		return (OR);
	if (s[0] == '&')
		return (BACKGROUND);
	if (ft_strncmp(">", s, 1) == 0 || ft_strncmp("<", s, 1) == 0)
		return (REDIRECT);
	if (ft_strncmp("|", s, 1) == 0)
		return (PIPE);
	if (*s == '(')
		return (OPEN_PAREN);
	if (*s == ')')
		return (CLOSE_PAREN);
	return (WORD);
}
