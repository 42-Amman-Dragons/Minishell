/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 12:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_has_quotes(char *word)
{
	while (*word)
	{
		if (*word == '\'' || *word == '"')
			return (1);
		word++;
	}
	return (0);
}

static void	append_expanded(char *arg, t_list **words, t_minishell *shell)
{
	char	*expanded;
	t_list	*node;

	expanded = expand_word(arg, shell->env, shell->exit_status);
	if (!expanded || (!word_has_quotes(arg) && !expanded[0]))
	{
		free(expanded);
		return ;
	}
	if (!word_has_quotes(arg) && ft_strchr(expanded, ' '))
	{
		ft_lstadd_back(words, split_to_list(expanded));
		return ;
	}
	node = ft_lstnew(expanded);
	if (!node)
	{
		free(expanded);
		return ;
	}
	ft_lstadd_back(words, node);
}

static char	**list_to_new_args(t_list *words)
{
	char	**args;
	t_list	*cur;
	t_list	*tmp;
	int		i;

	args = ft_calloc(ft_lstsize(words) + 1, sizeof(char *));
	if (!args)
	{
		ft_lstclear(&words, free);
		return (NULL);
	}
	i = 0;
	cur = words;
	while (cur)
	{
		args[i++] = (char *)cur->content;
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	return (args);
}

char	**build_expanded_args(char **args, t_minishell *shell)
{
	t_list	*words;
	int		i;

	words = NULL;
	i = 0;
	while (args[i])
	{
		append_expanded(args[i], &words, shell);
		i++;
	}
	if (!words)
		return (ft_calloc(1, sizeof(char *)));
	return (list_to_new_args(words));
}
