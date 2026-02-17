/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:31:51 by mabuqare          #+#    #+#             */
/*   Updated: 2026/02/16 00:31:53 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_operator(char *s, char **type)
{
	if (ft_strncmp(">>", s, 2) == 0)
	{
		*type = "DIR_OUT_FILE";
		return (2);
	}
	if (ft_strncmp("<<", s, 2) == 0)
	{
		*type = "DIR_IN_HEREDOC";
		return (2);
	}
	if (ft_strncmp("&&", s, 2) == 0)
	{
		*type = "AND";
		return (2);
	}
	if (ft_strncmp("||", s, 2) == 0)
	{
		*type = "OR";
		return (2);
	}
	if (ft_strncmp("|", s, 1) == 0)
	{
		*type = "PIPE";
		return (1);
	}
	if (ft_strncmp(">", s, 1) == 0)
	{
		*type = "DIR_OUT";
		return (1);
	}
	if (ft_strncmp("<", s, 1) == 0)
	{
		*type = "DIR_IN";
		return (1);
	}
	return (0);
}