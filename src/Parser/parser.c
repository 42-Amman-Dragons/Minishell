/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/25 01:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*parse_pipe_seq(t_list **cur, int *err)
{
	t_tree	*left;
	t_tree	*right;

	left = parse_cmd_or_sub(cur, err);
	if (*err)
		return (NULL);
	while (cur_type(cur) == PIPE)
	{
		advance(cur);
		right = parse_cmd_or_sub(cur, err);
		if (*err)
		{
			free_tree(left);
			return (NULL);
		}
		left = create_oper_node(NODE_PIPE, left, right);
	}
	return (left);
}

t_tree	*parse_logic_expr(t_list **cur, int *err)
{
	t_tree		*left;
	t_tree		*right;
	t_node_type	type;

	left = parse_pipe_seq(cur, err);
	if (*err)
		return (NULL);
	while (cur_type(cur) == AND || cur_type(cur) == OR)
	{
		if (cur_type(cur) == AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		advance(cur);
		right = parse_pipe_seq(cur, err);
		if (*err)
		{
			free_tree(left);
			return (NULL);
		}
		left = create_oper_node(type, left, right);
	}
	return (left);
}

t_tree	*build_ast(t_list *tokens)
{
	t_list	*cur;
	t_tree	*tree;
	int		err;

	if (!tokens)
		return (NULL);
	err = 0;
	cur = tokens;
	tree = parse_logic_expr(&cur, &err);
	if (err)
	{
		free_tree(tree);
		return (NULL);
	}
	if (cur)
	{
		syntax_error(cur, &err);
		free_tree(tree);
		return (NULL);
	}
	return (tree);
}
