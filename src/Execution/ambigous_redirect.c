/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambigous_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:04:53 by haya              #+#    #+#             */
/*   Updated: 2026/03/31 10:38:29 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_has_ambiguous_target(t_redir_data *rd)
{
	if (!rd->filename)
		return (1);
	if (rd->filename[0] == '\0')
		return (1);
	return (0);
}

int	print_ambiguous_redirect(void)
{
	ft_putstr_fd("minishell: ambiguous redirect\n", 2);
	return (-1);
}
