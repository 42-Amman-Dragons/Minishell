/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:14:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/04/05 16:53:54 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	add_token(t_list **head, char *line, int *i, int *err)
{
	t_token	*token;
	t_list	*new_node;

	token = create_token(&line[*i], i, err);
	if (!token)
		return (-1);
	new_node = ft_lstnew(token);
	if (!new_node)
	{
		free_token(token);
		return (-1);
	}
	ft_lstadd_back(head, new_node);
	return (0);
}

t_list	*tokenizer(char *line, int *err)
{
	t_list	*head;
	int		i;

	if (!line)
		return (NULL);
	head = NULL;
	i = 0;
	if (is_all_spaces(line))
	{
		*err = 0;
		return (NULL);
	}
	while (line[i])
	{
		skip_whitespaces(line, &i);
		if (!line[i])
			break ;
		if (add_token(&head, line, &i, err) == -1)
		{
			ft_lstclear(&head, free_token);
			return (NULL);
		}
	}
	return (head);
}
