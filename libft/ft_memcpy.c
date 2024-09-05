/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 20:44:59 by aabdou            #+#    #+#             */
/*   Updated: 2021/12/03 14:52:50 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void *ft_memcpy(void *dst, const void *src, size_t n)
{
	char *p;
	unsigned char *c_dst;
	const unsigned char *c_src;

	p = dst;
	c_dst = (unsigned char *)dst;
	c_src = (const unsigned char *)src;

	if (!p && !dst && !src)
		return (NULL);
	while (n--)
		*c_dst++ = *c_src++;
	return (p);
}
