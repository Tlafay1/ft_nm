/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:01:40 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/30 18:04:58 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
	Error message when opening the file
*/

int	nm_error(char *prog_name, char *path, char *msg)
{
	printf("%s: %s: %s\n", prog_name, path, msg);
	return (1);
}


/*
	Error returned when a syscall failed
*/

int	syscall_error(char *prog_name, char *path)
{
	printf("%s: '%s'", prog_name, path);
	ft_perror(NULL);
	return (1);
}

