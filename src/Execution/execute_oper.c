/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_oper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haya <haya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:27:58 by haya              #+#    #+#             */
/*   Updated: 2026/03/05 12:41:38 by haya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	handel_left_oper(t_tree *node, t_minishell *shell)
// {
// 	pid_t	left_id;
// 	int		status;
// 	int		left_exit_code;

// 	left_exit_code = 0;
// 	left_id = fork();
// 	status = 0;
// 	if (left_id == -1)
// 	{
// 		perror("PIPE ERROR: ");
// 		return(1);
// 	}
// 	else if (left_id == 0)
// 	{
// 		exit(exec_tree(node->data.oper.left, shell));
// 	}
// 	waitpid(left_id, &status, 0);
// 	if (WIFEXITED(status))
// 		left_exit_code = WEXITSTATUS(status);
// 	return (left_exit_code);
// }

// static int	handle_right_oper(t_tree *node, t_minishell *shell)
// {
// 	pid_t	right_id;
// 	int		status;
// 	int		right_exit_code;

// 	right_id = fork();
// 	if (right_id == -1)
// 	{
// 		perror("PIPE ERROR: ");
// 		return(1);
// 	}
// 	else if (right_id == 0)
// 	{
// 		exit(exec_tree(node->data.oper.right, shell));
// 	}
// 	waitpid(right_id, &status, 0);
// 	if (WIFEXITED(status))
// 		right_exit_code = WEXITSTATUS(status);
// 	return (right_exit_code);
// }

int	exec_and_or(t_tree *node, t_minishell *shell)
{	
	int left_exit_code;
	int right_exit_code;

	left_exit_code = exec_tree(node->data.oper.left, shell);
	if (node->type == NODE_AND && left_exit_code != 0)
		return(left_exit_code);
	else if (node->type == NODE_OR && left_exit_code == 0)
		return(left_exit_code);
	right_exit_code = exec_tree(node->data.oper.right, shell);
	return(right_exit_code);
}
