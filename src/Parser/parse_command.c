/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 17:53:35 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_data	*build_redir(t_list **cur, t_token *rtok, int *err)
{
	t_redir_data	*redir;

	redir = malloc(sizeof(t_redir_data));
	if (!redir)
	{
		*err = 1;
		return (NULL);
	}
	redir->mode = rtok->data.redir.mode;
	redir->filename = ft_strdup(((t_token *)(*cur)->content)->data.word.value);
	if (!redir->filename)
	{
		free(redir);
		*err = 1;
		return (NULL);
	}
	redir->heredoc_expand = 0;
	redir->heredoc_fd = -1;
	if (redir->mode == DIR_IN_HEREDOC && !ft_strchr(redir->filename, '\'')
			&& !ft_strchr(redir->filename, '"'))
		redir->heredoc_expand = 1;
	return (redir);
}

int	parse_redir(t_list **cur, t_list **redirs, int *err)
{
	t_redir_data	*redir;
	t_token			*redir_tok;
	t_list			*node;

	redir_tok = advance(cur);
	if (cur_type(cur) != WORD)
	{
		syntax_error(*cur, err);
		return (1);
	}
	redir = build_redir(cur, redir_tok, err);
	if (!redir)
		return (1);
	node = ft_lstnew(redir);
	if (!node)
	{
		free(redir->filename);
		free(redir);
		*err = 1;
		return (1);
	}
	ft_lstadd_back(redirs, node);
	advance(cur);
	return (0);
}

t_tree	*parse_subshell(t_list **cur, int *err)
{
	t_tree	*child;
	t_list	*redirs;

	advance(cur);
	child = parse_logic_expr(cur, err);
	if (*err)
		return (NULL);
	if (cur_type(cur) != CLOSE_PAREN)
	{
		syntax_error(*cur, err);
		free_tree(child);
		return (NULL);
	}
	advance(cur);
	redirs = NULL;
	while (!*err && cur_type(cur) == REDIRECT)
		parse_redir(cur, &redirs, err);
	if (*err)
	{
		free_tree(child);
		ft_lstclear(&redirs, free_redir);
		return (NULL);
	}
	return (create_subshell_node(child, redirs, err));
}

t_tree	*parse_cmd_or_sub(t_list **cur, int *err)
{
	if (cur_type(cur) == OPEN_PAREN)
		return (parse_subshell(cur, err));
	return (parse_simple_cmd(cur, err));
}
