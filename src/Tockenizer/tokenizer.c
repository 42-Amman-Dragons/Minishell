/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:14:20 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/16 00:31:16 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*This function checks if the character is a white space variant*/
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/*
This function skips i number of white spaces
(changes the value inside the i address)
*/
void	skip_whitespaces(char *ptr, int *i)
{
	while (ptr[*i])
	{
		if (is_whitespace(ptr[*i]))
			(*i)++;
	}
}

/*This function returns a LL of unexpanded tokens*/
void	*tokenizer(char *line)
{
	t_list *head;
	t_list *new_node;
	int i;
	char *type;
	int op_len;
	char *content;

	if (!line)
		return (NULL);
	head = NULL;
	i = 0;
	while (line[i])
	{
		skip_whitespaces(&line[i], &i);
		if (!line[i])
			break ;
		if (is_operator(&line[i], &type);)
			= is_operator(&line[i], &type);
		if (op_len > 0)
		{
			content = ft_substr(line, i, op_len);
			new_node = create_token_node(content, type);
			i += op_len;
		}
		else
		{
			// what about the difference between the filename and the normal comand?
			content = extract_word(line, &i);
			new_node = create_token_node(content, "WORD");
		}
		if (new_node)
		{
			ft_lstadd_back(&head, new_node);
			ft_printf("Token: %s, Type: %s\n",
				((t_token *)new_node->content)->content,
				((t_token *)new_node->content)->type);
		}
		else
		{
			if (content)
				free(content);
		}
	}
	return (head);
}