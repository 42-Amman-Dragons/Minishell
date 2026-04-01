/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:44:52 by haya              #+#    #+#             */
/*   Updated: 2026/03/31 10:46:42 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_eof_warning(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by"
		" end-of-file (wanted `",
		2);
	ft_putstr_fd(limiter, 2);
	ft_putstr_fd("')\n", 2);
}
