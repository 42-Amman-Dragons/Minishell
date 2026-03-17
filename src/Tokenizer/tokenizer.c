/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:14:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 05:29:47 by mabuqare         ###   ########.fr       */
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

t_token	*creat_token(char *str, int *i)
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

static int	add_token(t_list **head, char *line, int *i)
{
	t_token	*token;
	t_list	*new_node;

	token = creat_token(&line[*i], i);
	if (!token)
	{
		ft_lstclear(head, free_token);
		return (0);
	}
	new_node = ft_lstnew(token);
	if (!new_node)
	{
		free_token(token);
		ft_lstclear(head, free_token);
		return (0);
	}
	ft_lstadd_back(head, new_node);
	return (1);
}

t_list	*tokenizer(char *line)
{
	t_list	*head;
	int		i;

	if (!line)
		return (NULL);
	head = NULL;
	i = 0;
	while (line[i])
	{
		skip_whitespaces(line, &i);
		if (!line[i])
			break ;
		if (!add_token(&head, line, &i))
			return (NULL);
	}
	return (head);
}
