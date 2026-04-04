/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sigquit_if_needed.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 18:11:58 by haya              #+#    #+#             */
/*   Updated: 2026/04/04 18:12:21 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_sigquit_if_needed(int status, t_minishell *shell)
{
	int	err;

	err = child_exit_status(status);
	if (shell->is_child == 0 && (err == 131 || (WIFSIGNALED(status)
				&& WTERMSIG(status) == SIGQUIT)))
		write(2, "Quit (core dumped)\n", 19);
}
