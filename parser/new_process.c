/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:39:33 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/08 13:12:33 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	new_process(t_lexer *tokens)
{
	while (tokens)
	{
		if (tokens->token == 1)
		{
			if (handle_input_redirect(tokens) < 0)
				return ;
			tokens = tokens->next;
		}
		else if (tokens->token == 4)
		{
			if (handle_output_redirect(tokens) < 0)
				return ;
			tokens = tokens->next;
		}
		if (tokens)
			tokens = tokens->next;
	}
}
