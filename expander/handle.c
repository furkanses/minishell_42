/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:06:53 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/08 12:19:30 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	handle_dollar_quote(char *cmd, int i, int *new_len)
{
	if (cmd[i + 2] == '\0')
		(*new_len) += 1;
	return (i + 1);
}

void	counter_expand(int *total, int *i)
{
	(*i)++;
	(*total)++;
}
