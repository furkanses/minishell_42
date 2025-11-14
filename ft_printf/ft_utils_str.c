/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:44:04 by mdinc             #+#    #+#             */
/*   Updated: 2024/12/04 20:44:04 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putchar(char c)
{
	if (write(2, &c, 1) == -1)
		return (-1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		if (write(2, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	while (str[i])
	{
		if (ft_putchar(str[i]) == -1)
			return (-1);
		i++;
	}
	return (i);
}
