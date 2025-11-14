/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 02:27:51 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 02:28:28 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_multi_flags(t_ast *node)
{
	t_ast	*temp;

	temp = node;
	while (temp && temp->left)
	{
		temp->left->multi_flag = 1;
		temp = temp->left;
	}
}

int	is_redirection_node(t_ast *node)
{
	t_ast	*temp;

	temp = node;
	while (temp)
	{
		if (temp->type == NODE_HEREDOC)
			return (1);
		if (temp->left)
			temp = temp->left;
		else
			break ;
	}
	return (0);
}
