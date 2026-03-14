/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_factory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 20:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/13 00:19:09 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_token	*create_pipe_token(char *str, int *i)
{
	t_token	*token;

	(void)str;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = PIPE;
	(*i)++;
	return (token);
}

t_token	*create_background_token(char *str, int *i)
{
	t_token	*token;

	(void)str;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = BACKGROUND;
	(*i)++;
	return (token);
}

t_token	*create_redirect_token(char *str, int *i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = REDIRECT;
	token->data.redir.mode = identify_redirection_mode(str, i);
	return (token);
}

t_token	*create_and_or_token(char *str, int *i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (ft_strncmp("&&", str, 2) == 0)
		token->type = AND;
	else if (ft_strncmp("||", str, 2) == 0)
		token->type = OR;
	(*i) += 2;
	return (token);
}

t_token	*create_paren_token(char *str, int *i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (*str == '(')
		token->type = OPEN_PAREN;
	else
		token->type = CLOSE_PAREN;
	(*i)++;
	return (token);
}

t_token	*create_word_token(char *str, int *i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = WORD;
	token->data.word.value = extract_word(str, i);
	if (!token->data.word.value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}
