/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:17:32 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/16 11:58:54 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	is_operator(char *s, char **type)
{
	if (ft_strncmp(">>", s, 2) == 0)
	{
		*type = "DIR_OUT_FILE";
		return (2);
	}
	if (ft_strncmp("<<", s, 2) == 0)
	{
		*type = "DIR_IN_HEREDOC";
		return (2);
	}
	if (ft_strncmp("&&", s, 2) == 0)
	{
		*type = "AND";
		return (2);
	}
	if (ft_strncmp("||", s, 2) == 0)
	{
		*type = "OR";
		return (2);
	}
	if (ft_strncmp("|", s, 1) == 0)
	{
		*type = "PIPE";
		return (1);
	}
	if (ft_strncmp(">", s, 1) == 0)
	{
		*type = "DIR_OUT";
		return (1);
	}
	if (ft_strncmp("<", s, 1) == 0)
	{
		*type = "DIR_IN";
		return (1);
	}
	return (0);
}

t_list	*create_token_node(char *content, char *type)
{
	t_token	*new_token;
	t_list	*node;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = content;
	new_token->type = type;
	node = ft_lstnew(new_token);
	if (!node)
	{
		free(new_token);
		return (NULL);
	}
	return (node);
}

char	*extract_word(char *line, int *i)
{
	int		start;
	char	quote;
	char	*word;
	char	*op_check;

	start = *i;
	// the op_check is not initialized and not updated in the loop
	while (line[*i] && !is_whitespace(line[*i]) && !is_operator(&line[*i],
			&op_check))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			quote = line[*i];
			(*i)++;
			while (line[*i] && line[*i] != quote)
				(*i)++;
		}
		if (line[*i])
			(*i)++;
	}
	word = ft_substr(line, start, *i - start);
	return (word);
}

void	*tokeniztion(char *line)
{
	t_list	*head;
	t_list	*new_node;
	int		i;
	char	*type;
	int		op_len;
	char	*content;

	if (!line)
		return (NULL);
	head = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && is_whitespace(line[i]))
			i++;
		if (!line[i])
			break ;
		op_len = is_operator(&line[i], &type);
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
			// ft_printf("Token: %s, Type: %s\n",
			// 	((t_token *)new_node->content)->content,
			// 	((t_token *)new_node->content)->type);
		}
		else
		{
			if (content)
				free(content);
		}
	}
	return (head);
}
