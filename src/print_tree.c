/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:22:04 by haya              #+#    #+#             */
/*   Updated: 2026/03/08 11:22:57 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*node_type_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("CMD");
	if (type == NODE_PIPE)
		return ("PIPE");
	if (type == NODE_AND)
		return ("AND");
	if (type == NODE_OR)
		return ("OR");
	return ("SUBSHELL");
}

void	print_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	printf("[");
	while (arr[i])
	{
		printf("%s", arr[i]);
		if (arr[i + 1])
			printf(", ");
		i++;
	}
	printf("]");
}

static void	print_redirs(t_list *redirs)
{
	t_redir_data	*rd;

	while (redirs)
	{
		rd = (t_redir_data *)redirs->content;
		if (rd->mode == DIR_IN_FILE)
			printf(" < %s", rd->filename);
		else if (rd->mode == DIR_OUT_TRUNC)
			printf(" > %s", rd->filename);
		else if (rd->mode == DIR_OUT_APPEND)
			printf(" >> %s", rd->filename);
		else
			printf(" << %s", rd->filename);
		redirs = redirs->next;
	}
}

static void	print_node(t_tree *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("[%s] ", node_type_str(node->type));
	if (node->type == NODE_CMD)
	{
		print_arr(node->data.cmd.args);
		print_redirs(node->data.cmd.redirections);
		printf("\n");
	}
	else if (node->type == NODE_SUBSHELL)
	{
		print_redirs(node->data.subshell.redirections);
		printf("\n");
		print_node(node->data.subshell.child, depth + 1);
	}
	else
	{
		printf("\n");
		print_node(node->data.oper.left, depth + 1);
		print_node(node->data.oper.right, depth + 1);
	}
}

void	print_tree(t_tree *head)
{
	if (!head)
	{
		printf("(empty tree)\n");
		return ;
	}
	print_node(head, 0);
}

void	ft_lst_print(t_list *lst)
{
	t_token	*tok;

	if (!lst)
		return ;
	while (lst)
	{
		tok = (t_token *)lst->content;
		if (tok->type == WORD)
			printf("WORD(%s)->", tok->data.word.value);
		else
			printf("OP(%d)->", tok->type);
		lst = lst->next;
	}
	printf("NULL\n");
}