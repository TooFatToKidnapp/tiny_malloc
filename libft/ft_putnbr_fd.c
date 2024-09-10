/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 18:37:58 by aabdou            #+#    #+#             */
/*   Updated: 2021/11/19 19:52:31 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_module_fd(size_t n)
{
	if (n < 0)
		return (n * (-1));
	else
		return (n);
}

void	ft_putnbr_fd(size_t n, int fd)
{
	size_t	num;

	if (n < 0)
		ft_putchar_fd('-', fd);
	num = ft_module_fd(n);
	if (num >= 10)
	{
		ft_putnbr_fd(num / 10, fd);
		ft_putnbr_fd(num % 10, fd);
	}
	else
		ft_putchar_fd(num + '0', fd);
}
