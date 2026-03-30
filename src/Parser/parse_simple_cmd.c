/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/30 11:40:14 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**list_to_args(t_list *words)
{
	char	**args;
	int		i;
	t_list	*tmp;

	if (!words)
		return (NULL);
	args = ft_calloc(ft_lstsize(words) + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	tmp = words;
	while (tmp)
	{
		args[i] = (char *)tmp->content;
		tmp = tmp->next;
		i++;
	}
	return (args);
}

static int	add_word(t_list **words, t_list **cur, int *err)
{
	char	*word;
	t_list	*node;

	word = ft_strdup(((t_token *)(*cur)->content)->data.word.value);
	if (!word)
	{
		*err = 1;
		return (1);
	}
	node = ft_lstnew(word);
	if (!node)
	{
		free(word);
		*err = 1;
		return (1);
	}
	ft_lstadd_back(words, node);
	advance(cur);
	return (0);
}

/* frees only the t_list nodes, NOT the char* content.
** used after list_to_args() succeeds: strings are transferred
** into args[] by pointer, so ft_lstclear(&words, free) would
** double-free them when free_args(args) is called later. */
static void	free_words_list(t_list *words)
{
	t_list	*tmp;

	while (words)
	{
		tmp = words->next;
		free(words);
		words = tmp;
	}
}

static int	collect_tokens(t_list **cur, t_list **words, t_list **redirs,
		int *err)
{
	while (cur_type(cur) == WORD || cur_type(cur) == REDIRECT)
	{
		if (cur_type(cur) == WORD)
			add_word(words, cur, err);
		else
			parse_redir(cur, redirs, err);
		if (*err && *err != 20)
		{
			ft_lstclear(words, free);
			ft_lstclear(redirs, free_redir);
			return (1);
		}
	}
	return (0);
}

t_tree	*parse_simple_cmd(t_list **cur, int *err)
{
	t_list	*words;
	t_list	*redirs;
	char	**args;

	words = NULL;
	redirs = NULL;
	if (collect_tokens(cur, &words, &redirs, err) != 0)
		return (NULL);
	if (!words && !redirs)
	{
		syntax_error(*cur, err); 
		return (NULL);
	}
	args = list_to_args(words);
	if (!args && words)
	{
		ft_lstclear(&words, free);
		ft_lstclear(&redirs, free_redir);
		*err = 1;
		return (NULL);
	}
	free_words_list(words);
	return (create_cmd_node(args, redirs, err));
}
