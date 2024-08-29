/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 20:47:06 by aabdou            #+#    #+#             */
/*   Updated: 2021/12/01 20:47:09 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	n;
	size_t	dst_len;
	size_t	src_len;

	i = 0;
	n = 0;
	src_len = ft_strlen(src);
	if (!dst && src && dstsize == 0)
		return (src_len);
	dst_len = ft_strlen(dst);	
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	while (dst[n] != 0)
		n++;
	while ((dst_len + i) < (dstsize - 1) && src[i] != 0)
	{
		dst[n] = src[i];
		n++;
		i++;
	}
	dst[n] = '\0';
	return (dst_len + src_len);
}
