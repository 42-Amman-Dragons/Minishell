/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 16:26:42 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/04 12:29:45 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_boundary(char *str, int *err)
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
		*err = 2;
		return (-1);
	}
	return (y);
}

char	*extract_word(char *str, int *i, int *err)
{
	char	*word;
	int		len;

	len = word_boundary(str, err);
	if (len < 0)
		return (NULL);
	word = ft_substr(str, 0, len);
	(*i) += len;
	return (word);
}

t_token	*create_word_token(char *str, int *i, int *err)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = WORD;
	token->data.word.value = extract_word(str, i, err);
	if (!token->data.word.value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}
