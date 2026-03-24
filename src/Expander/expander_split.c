/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 12:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/24 12:00:00 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*build_list(char **parts)
{
	t_list	*result;
	t_list	*node;
	int		i;

	result = NULL;
	i = 0;
	while (parts[i])
	{
		node = ft_lstnew(parts[i]);
		if (!node)
		{
			ft_lstclear(&result, free);
			while (parts[i])
				free(parts[i++]);
			free(parts);
			return (NULL);
		}
		ft_lstadd_back(&result, node);
		i++;
	}
	free(parts);
	return (result);
}

t_list	*split_to_list(char *s)
{
	char	**parts;

	parts = ft_split(s, ' ');
	free(s);
	if (!parts)
		return (NULL);
	return (build_list(parts));
}
