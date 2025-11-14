/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:48:38 by mdinc             #+#    #+#             */
/*   Updated: 2024/12/04 20:48:38 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putint(int n)
{
	long	nb;
	int		len;
	int		tmp;
	char	c;

	len = 0;
	nb = (long)n;
	if (nb < 0)
	{
		if (write(2, "-", 1) == -1)
			return (-1);
		nb *= -1;
		len++;
	}
	if (nb >= 10)
	{
		tmp = ft_putint(nb / 10);
		if (tmp == -1)
			return (-1);
		len += tmp;
	}
	c = nb % 10 + '0';
	if (write(2, &c, 1) == -1)
		return (-1);
	return (len + 1);
}

int	ft_putuint(unsigned int n)
{
	long	nb;
	int		len;
	int		tmp;
	char	c;

	len = 0;
	nb = (long)n;
	if (nb >= 10)
	{
		tmp = ft_putuint(nb / 10);
		if (tmp == -1)
			return (-1);
		len += tmp;
	}
	c = ((nb % 10) + '0');
	if (write(2, &c, 1) == -1)
		return (-1);
	len++;
	return (len);
}
