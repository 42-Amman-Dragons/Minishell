/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_factory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:14:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 05:29:43 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	free_token(void *ptr)
{
	t_token	*token;

	if (!ptr)
		return ;
	token = (t_token *)ptr;
	if (token->type == WORD)
		free(token->data.word.value);
	free(token);
}

t_token	*create_token(char *str, int *i)
{
	t_tokenType	type;

	type = identify_token(str);
	if (type == PIPE)
		return (create_pipe_token(str, i));
	else if (type == BACKGROUND)
		return (create_background_token(str, i));
	else if (type == REDIRECT)
		return (create_redirect_token(str, i));
	else if (type == AND || type == OR)
		return (create_and_or_token(str, i));
	else if (type == OPEN_PAREN || type == CLOSE_PAREN)
		return (create_paren_token(str, i));
	else
		return (create_word_token(str, i));
}
