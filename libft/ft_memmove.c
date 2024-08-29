/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 20:53:18 by aabdou            #+#    #+#             */
/*   Updated: 2021/12/03 14:53:06 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dst_str;
	const char	*src_str;

	dst_str = dst;
	src_str = src;
	if (dst > src)
	{
		while (len--)
		{
			*(dst_str + len) = *(src_str + len);
		}
	}
	else
		ft_memcpy(dst_str, src_str, len);
	return (dst);
}
