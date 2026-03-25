/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-lawa <hal-lawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:30:32 by haya              #+#    #+#             */
/*   Updated: 2026/03/25 13:04:08 by hal-lawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_prompt(t_minishell *shell)
{
	print_welcome_message();
	shell->prompt = get_prompt(shell);
}

int	init_terminal(t_minishell *shell)
{
	if (!shell->is_interactive)
		return (0);
	if (tcgetattr(STDIN_FILENO, &(shell->original_termos)) == -1)
		return (-1);
	shell->new_termos = shell->original_termos;
	shell->new_termos.c_lflag &= ~(ECHOCTL);
	shell->new_termos.c_cc[VMIN] = 1;
	shell->new_termos.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(shell->new_termos)) == -1)
		return (-1);
	return (0);
}

t_minishell	*init_shell(void)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->openfiles = NULL;
	return (shell);
}

int	init_interactive_shell(t_minishell *shell)
{
	if (init_terminal(shell) != 0)
		return (-1);
	set_signals_prompt();
	if (load_history(shell) != 0)
		return (-1);
	init_prompt(shell);
	return (0);
}
