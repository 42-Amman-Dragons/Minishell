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

static int	add_token(t_list **head, char *line, int *i)
{
	t_token	*token;
	t_list	*new_node;

	token = create_token(&line[*i], i);
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
