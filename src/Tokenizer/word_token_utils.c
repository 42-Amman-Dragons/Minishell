/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 16:26:42 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/13 00:19:52 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_boundary(char *str)
{
	int	y;
	int	squote_flag;
	int	dquote_flag;

	y = 0;
	squote_flag = 0;
	dquote_flag = 0;
	while (str[y] && (squote_flag || dquote_flag || !is_whitespace(str[y])))
	{
		if (str[y] == '"' && !squote_flag)
			dquote_flag = !dquote_flag;
		else if (str[y] == '\'' && !dquote_flag)
			squote_flag = !squote_flag;
		if (!squote_flag && !dquote_flag && identify_token(&str[y]) != WORD)
			break ;
		y++;
	}
	if (squote_flag || dquote_flag)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (-1);
	}
	return (y);
}

char	*extract_word(char *str, int *i)
{
	char	*word;
	int		len;

	len = word_boundary(str);
	if (len < 0)
		return (NULL);
	word = ft_substr(str, 0, len);
	(*i) += len;
	return (word);
}
