/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strerror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:54:02 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/30 17:56:10 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


char *ft_strerror (int errnoval)
{
	char *msg;

	if ((errnoval < 0) || (errnoval >= sys_nerr))
		msg = NULL;
	else
		msg = (char *) sys_errlist[errnoval];

	return (msg);
}