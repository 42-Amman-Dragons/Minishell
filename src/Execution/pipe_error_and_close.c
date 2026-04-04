/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_error_and_close.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 18:22:44 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 18:23:54 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_error_and_close(int *temp_std)
{
	perror("PIPE ERROR: ");
	safe_close(&temp_std[0], "close error");
	safe_close(&temp_std[1], "close error");
	return (1);
}
