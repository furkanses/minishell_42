/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:29:34 by mdinc             #+#    #+#             */
/*   Updated: 2024/12/06 14:01:23 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>

static int	ft_handle_format(va_list *args, char c)
{
	if (c == 'c')
		return (ft_putchar(va_arg(*args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(*args, char *)));
	else if (c == 'p')
		return (ft_putptr(va_arg(*args, unsigned long)));
	else if (c == 'd' || c == 'i')
		return (ft_putint(va_arg(*args, int)));
	else if (c == 'u')
		return (ft_putuint(va_arg(*args, unsigned int)));
	else if (c == 'x' || c == 'X')
		return (ft_puthex(va_arg(*args, unsigned int), c));
	else if (c == '%')
		return (ft_putchar('%'));
	else
		return (-1);
}

static int	ft_format(va_list *args, const char *format)
{
	int	temp;
	int	total_len;
	int	i;

	i = -1;
	total_len = 0;
	while (format[++i] != '\0')
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == '\0')
				return (-1);
			temp = ft_handle_format(args, format[++i]);
			if (temp == -1)
				return (-1);
			total_len += temp;
		}
		else
		{
			if (ft_putchar(format[i]) == -1)
				return (-1);
			total_len++;
		}
	}
	return (total_len);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		total_len;

	if (!format)
		return (-1);
	total_len = 0;
	va_start(args, format);
	total_len = ft_format(&args, format);
	if (total_len == -1)
	{
		va_end(args);
		return (-1);
	}
	va_end(args);
	return (total_len);
}
