/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 09:52:18 by haya              #+#    #+#             */
/*   Updated: 2026/01/29 10:04:42 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_all(t_minishell *shell)
{
    if(!shell)
        return ;
    if(shell->line)
        free(shell->line);
    if(shell->history)
    {
        ft_lstclear(&(shell->history), free);
        rl_clear_history();
    }
    if(shell->prompt)
        free(shell->prompt);
    free(shell);
}