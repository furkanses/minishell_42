/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:27:10 by mdinc             #+#    #+#             */
/*   Updated: 2024/12/05 10:27:10 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putptr_helper(unsigned long n)
{
	char	hex;
	int		len;
	int		temp;

	len = 0;
	if (n >= 16)
	{
		temp = ft_putptr_helper(n / 16);
		if (temp == -1)
			return (-1);
		len += temp;
	}
	hex = "0123456789abcdef"[n % 16];
	if (ft_putchar(hex) == -1)
		return (-1);
	return (len + 1);
}

int	ft_putptr(unsigned long n)
{
	int	len;
	int	temp;

	if (n == 0)
	{
		if (ft_putstr("(nil)") == -1)
			return (-1);
		return (5);
	}
	if (ft_putchar('0') == -1 || ft_putchar('x') == -1)
		return (-1);
	len = 2;
	temp = ft_putptr_helper(n);
	if (temp == -1)
		return (-1);
	len += temp;
	return (len);
}
