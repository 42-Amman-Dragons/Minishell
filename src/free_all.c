/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:18 by haya              #+#    #+#             */
/*   Updated: 2026/01/29 15:11:17 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_all(t_minishell *shell)
{
    if (!shell)
        return;
    tcsetattr(STDIN_FILENO, TCSANOW, &(shell->original_termos));
    if (shell->history)
        ft_lstclear(&(shell->history), free);
    if (shell->prompt)
        free(shell->prompt);
    free(shell);
}