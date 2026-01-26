/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:58:41 by hal-lawa          #+#    #+#             */
/*   Updated: 2025/08/26 15:54:41 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	clear_all(void **result, t_list **newlist, void (*del)(void *))
{
	del(*result);
	ft_lstclear(newlist, del);
	*newlist = NULL;
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlist;
	t_list	*node;
	void	*result;

	newlist = NULL;
	if (!del)
		return (NULL);
	while (lst)
	{
		result = f(lst -> content);
		if (!result)
		{
			clear_all(&result, &newlist, del);
			return (NULL);
		}
		node = ft_lstnew(result);
		if (!node)
		{
			clear_all(&result, &newlist, del);
			return (NULL);
		}
		ft_lstadd_back(&newlist, node);
		lst = lst -> next;
	}
	return (newlist);
}
