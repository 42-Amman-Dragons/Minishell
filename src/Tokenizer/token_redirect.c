/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 16:26:38 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/17 05:29:40 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dir_mode	identify_redirection_mode(char *str, int *i)
{
	if (ft_strncmp("<<", str, 2) == 0)
	{
		(*i) += 2;
		return (DIR_IN_HEREDOC);
	}
	else if (ft_strncmp(">>", str, 2) == 0)
	{
		(*i) += 2;
		return (DIR_OUT_APPEND);
	}
	else if (ft_strncmp("<", str, 1) == 0)
	{
		(*i)++;
		return (DIR_IN_FILE);
	}
	else if (ft_strncmp(">", str, 1) == 0)
	{
		(*i)++;
		return (DIR_OUT_TRUNC);
	}
	return (DIR_IN_FILE);
}

t_token	*create_redirect_token(char *str, int *i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = REDIRECT;
	token->data.redir.mode = identify_redirection_mode(str, i);
	token->data.redir.filename = NULL;
	token->data.redir.heredoc_expand = 0;
	token->data.redir.heredoc_fd = -1;
	return (token);
}
