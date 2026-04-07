/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:44:52 by hal-lawa          #+#    #+#             */
/*   Updated: 2026/04/07 09:31:48 by hal-lawa         ###   ########.fr       */
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
